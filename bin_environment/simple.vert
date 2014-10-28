#version 400 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

out Data {
    vec3 normal;
    vec2 texcoord;
} outData;

//out vec3 tcNormal;
//out vec2 tcTexcoord;


void main( void )
{
    gl_Position =  vec4(vertex, 1);

    outData.normal = normalize(normal);
    outData.texcoord = texcoord;

//    tcNormal = normalize(normal);
//    tcTexcoord = texcoord;
}
