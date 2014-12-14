#version 400 core

in vec3 skyboxTexcoord;

uniform sampler2D sphericalMapSampler;

layout(location = 0) out vec4 fragColor;

#define M_PI 3.1415926535897932384626433832795

void main( void )
{
    float norm = inversesqrt(skyboxTexcoord.x*skyboxTexcoord.x + skyboxTexcoord.y*skyboxTexcoord.y + skyboxTexcoord.z*skyboxTexcoord.z);

    float DDx = skyboxTexcoord.x * norm;
    float DDy = skyboxTexcoord.y * norm;
    float DDz = skyboxTexcoord.z * norm;

    float r = 0.159154943 * acos(DDz) * inversesqrt(DDx*DDx + DDy*DDy);

    float sb_u = 0.5 + DDx * r;
    float sb_v = 0.5 + DDy * r;

    fragColor = texture(sphericalMapSampler, vec2(sb_u, sb_v));
}
