#version 400 core

uniform sampler2D texSampler;

uniform sampler2D brightSampler;

uniform int screenWidth;
uniform int screenHeight;

layout(location = 0, index = 0) out vec4 fragColor;

#define approx(x, val) (val - 0.00001 < x && x < val + 0.00001)

void main( void )
{
    vec2 screenTexCoord = vec2(gl_FragCoord.x/screenWidth, gl_FragCoord.y/screenHeight);

    vec3 rgbColor = texture(texSampler, screenTexCoord).rgb;

    vec3 brightColor = texture(brightSampler, screenTexCoord).rgb;

//    brightColor = vec3(0);

    fragColor = vec4(rgbColor + brightColor, 1);
}
