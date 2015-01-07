#version 400 core

uniform sampler2D texSampler;

uniform int screenWidth;
uniform int screenHeight;

uniform float avLum;
uniform float gamma;
uniform float keyValue;

layout(location = 0, index = 0) out vec4 fragColor;

#define approx(x, val) (val - 0.00001 < x && x < val + 0.00001)

// CLASSIC ROW MAJOR
//const mat3 RGBtoXYZ = mat3(
//    0.4124, 0.3576, 0.1805,
//    0.2126, 0.7152, 0.0722,
//    0.0193, 0.1192, 0.9505
//);

// "CONVERT" TO COLUMN MAJOR
const mat3 RGBtoXYZ = mat3(
    0.4124, 0.2126, 0.0193,
    0.3576, 0.7152, 0.1192,
    0.1805, 0.0722, 0.9505
);

vec3 RGBtoxyY(vec3 rgb)
{
    // Convert from RGB to XYZ
    float X = rgb.r * 0.4124 + rgb.g * 0.3576 + rgb.b * 0.1805;
    float Y = rgb.r * 0.2126 + rgb.g * 0.7152 + rgb.b * 0.0722;
    float Z = rgb.r * 0.0193 + rgb.g * 0.1192 + rgb.b * 0.9505;

    // Convert from XYZ to xyY
    float L = (X + Y + Z);

    //vec3 xyY
    return vec3(X / L, Y / L, Y);
}

vec3 xyYtoRGB(vec3 xyY)
{
    float x = xyY.x;
    float y = xyY.y;
    float Y = xyY.z;

    // Convert from xyY to XYZ
    float X = x * (Y / y);
    float Z = (1 - x - y) * (Y / y);

    // Convert from XYZ to RGB
    float R = X *  3.2406 + Y * -1.5372 + Z * -0.4986;
    float G = X * -0.9689 + Y *  1.8758 + Z *  0.0415;
    float B = X *  0.0557 + Y * -0.2040 + Z *  1.0570;

    return vec3(R, G, B);
}

float luminance(vec3 color) {
    return color.r * 0.2126 + color.g * 0.7152 + color.b * 0.0722;
}

void main( void )
{
    vec2 screenTexCoord = vec2(gl_FragCoord.x/screenWidth, gl_FragCoord.y/screenHeight);

    vec3 rgbColor = texture(texSampler, screenTexCoord).rgb;

//    vec3 xyzColor = RGBtoXYZ * rgbColor;

//    rgbColor *= 16;

//    vec3 xyYColor = RGBtoxyY(rgbColor);

    float Lw = luminance(rgbColor);

    float L = (keyValue / avLum) * Lw;

    float Ld = L / (1 + L);

    const float saturation = 1;

//    Ld = Lw;

//    xyYColor.z = Ld;

//    float r = pow((rgbColor.r / Lw), c) * Ld;
//    float g = pow((rgbColor.g / Lw), c) * Ld;
//    float b = pow((rgbColor.b / Lw), c) * Ld;

    vec3 rgbColorTonemapped = rgbColor * (Ld / Lw); //pow(pow(rgbColor * (Ld / Lw), vec3(saturation)), vec3(1/gamma));

//    rgbColorTonemapped = xyYtoRGB(xyYColor);

    rgbColorTonemapped = pow(rgbColorTonemapped, vec3(saturation));

    rgbColorTonemapped = pow(rgbColorTonemapped, vec3(1/gamma));

    fragColor = vec4(rgbColorTonemapped, 1);
}
