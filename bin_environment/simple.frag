#version 400 core

in Data {
    vec3 normal;
    vec2 texcoord;
} inData;

//in vec3 fNormal;
//in vec2 fTexcoord;

layout(location = 0, index = 0) out vec4 fragColor;

void main( void )
{
    vec3 finalColor = abs(max(vec3(-0.2f), normalize(inData.normal)));

    fragColor = vec4( finalColor, 1.0 );
//    fragColor = vec4( inData.texcoord, 0, 1.0 );
//    fragColor = vec4(1, 0, 0, 1);
}
