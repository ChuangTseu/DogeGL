#version 400 core

in vec3 skyboxTexcoord;

uniform samplerCube skyboxSampler;

layout(location = 0) out vec4 fragColor;

void main( void )
{
    //WARNING: Inverting skyboxTexcoord vector direction
    //My bet is on: I go Right handed, Cubemap is "left handed"
    //Watever, it "works" :p
//    fragColor = texture(skyboxSampler, skyboxTexcoord)*0.5 + vec4(skyboxTexcoord,1);
//    fragColor = vec4(skyboxTexcoord,1);

    fragColor = texture(skyboxSampler, skyboxTexcoord);

//    if (abs(skyboxTexcoord.y) > abs(skyboxTexcoord.x) && abs(skyboxTexcoord.y) > abs(skyboxTexcoord.z)) {
//        fragColor = texture(skyboxSampler, vec3(skyboxTexcoord.x, skyboxTexcoord.y, skyboxTexcoord.z));
//    }
//    else {
//        fragColor = texture(skyboxSampler, vec3(skyboxTexcoord.x, -skyboxTexcoord.y, skyboxTexcoord.z));
//    }
}
