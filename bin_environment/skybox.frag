#version 400 core

in vec3 skyboxTexcoord;

uniform samplerCube skyboxSampler;

layout(location = 0) out vec4 fragColor;

void main( void )
{
    //WARNING: Inverting skyboxTexcoord vector direction
    //My bet is on: I go Right handed, Cubemap is "left handed"
    //Watever, it "works" :p
    fragColor = texture(skyboxSampler, -skyboxTexcoord);
//    fragColor = vec4(1,0,0,1);
}
