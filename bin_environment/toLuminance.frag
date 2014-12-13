#version 400 core

uniform sampler2D texSampler;

uniform int screenWidth;
uniform int screenHeight;

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

void main( void )
{
    vec2 screenTexCoord = vec2(gl_FragCoord.x/screenWidth, gl_FragCoord.y/screenHeight);

    vec3 xyzColor = RGBtoXYZ * texture(texSampler, screenTexCoord).xyz;

//    xyzColor.y;

    fragColor = vec4(xyzColor, log(xyzColor.y + 1e-6));
}
