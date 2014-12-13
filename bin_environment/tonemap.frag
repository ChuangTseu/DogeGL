#version 400 core

uniform sampler2D texSampler;

uniform int screenWidth;
uniform int screenHeight;

uniform float logAvLum;
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

float luminance(vec3 color) {
    return color.r * 0.2126 + color.g * 0.7152 + color.b * 0.0722;
}

void main( void )
{
    vec2 screenTexCoord = vec2(gl_FragCoord.x/screenWidth, gl_FragCoord.y/screenHeight);

    vec3 rgbColor = texture(texSampler, screenTexCoord).rgb;

    vec3 xyzColor = RGBtoXYZ * rgbColor;

    float Lw = xyzColor.y;//luminance(rgbColor);

    float avLum = exp(logAvLum);

    float L = (keyValue / avLum) * Lw;

    float Ld = L / (1 + L);

    const float saturation = 1;

//    float r = pow((rgbColor.r / Lw), c) * Ld;
//    float g = pow((rgbColor.g / Lw), c) * Ld;
//    float b = pow((rgbColor.b / Lw), c) * Ld;

    vec3 rgbColorTonemapped = pow(pow(rgbColor * Lw, vec3(saturation)) * Ld, vec3(1/gamma));

    fragColor = vec4(rgbColorTonemapped, 1);


//    fragColor = vec4(vec3(rgbColor.y), 1);
//    fragColor = vec4(vec3(Ld), 1);



}
