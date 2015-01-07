#version 400 core

uniform sampler2D texSampler;

uniform int screenWidth;
uniform int screenHeight;

uniform float avLum;
uniform float gamma;
uniform float keyValue;

layout(location = 0, index = 0) out vec4 fragColor;

#define approx(x, val) (val - 0.00001 < x && x < val + 0.00001)

float luminance(vec3 color) {
    return color.r * 0.2126 + color.g * 0.7152 + color.b * 0.0722;
}

void main( void )
{
    vec2 screenTexCoord = vec2(gl_FragCoord.x/screenWidth, gl_FragCoord.y/screenHeight);

    vec3 rgbColor = texture(texSampler, screenTexCoord).rgb;

    vec3 brightOnly;

    if (rgbColor.r > 0.5) brightOnly.r = rgbColor.r;
    if (rgbColor.g > 0.5) brightOnly.g = rgbColor.g;
    if (rgbColor.b > 0.5) brightOnly.b = rgbColor.b;

    fragColor = vec4(brightOnly, 1);
}
