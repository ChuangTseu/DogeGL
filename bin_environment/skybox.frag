#version 400 core

in vec3 skyboxTexcoord;

uniform samplerCube cubeMapSampler;

layout(location = 0) out vec4 fragColor;

void main( void )
{
    fragColor = texture(cubeMapSampler, skyboxTexcoord);
}
