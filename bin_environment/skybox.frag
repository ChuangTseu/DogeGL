#version 400 core

in vec3 skyboxTexcoord;

uniform samplerCube skyboxSampler;

uniform sampler2D angularMapSampler;

layout(location = 0) out vec4 fragColor;

#define M_PI 3.1415926535897932384626433832795

void main( void )
{
//    fragColor = texture(skyboxSampler, skyboxTexcoord);

    float norm = inversesqrt(skyboxTexcoord.x*skyboxTexcoord.x + skyboxTexcoord.y*skyboxTexcoord.y + skyboxTexcoord.z*skyboxTexcoord.z);

    float DDx = skyboxTexcoord.x * norm;
    float DDy = skyboxTexcoord.y * norm;
    float DDz = skyboxTexcoord.z * norm;

    float r = 0.159154943 * acos(DDz) * inversesqrt(DDx*DDx + DDy*DDy);

    float sb_u = 0.5 + DDx * r;
    float sb_v = 0.5 + DDy * r;

//    float r = (1/M_PI) * acos(skyboxTexcoord.z) / (sqrt(skyboxTexcoord.x*skyboxTexcoord.x + skyboxTexcoord.y*skyboxTexcoord.y));

    fragColor = texture(angularMapSampler, vec2(sb_u, sb_v));

//    if (abs(skyboxTexcoord.y) > abs(skyboxTexcoord.x) && abs(skyboxTexcoord.y) > abs(skyboxTexcoord.z)) {
//        fragColor = texture(skyboxSampler, vec3(skyboxTexcoord.x, skyboxTexcoord.y, skyboxTexcoord.z));
//    }
//    else {
//        fragColor = texture(skyboxSampler, vec3(skyboxTexcoord.x, -skyboxTexcoord.y, skyboxTexcoord.z));
//    }
}
