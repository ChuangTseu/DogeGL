#version 400 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;
layout(location = 3) in vec3 tangent;

out Data {
    vec3 normal;
    vec2 texcoord;
    vec3 tangent;
} outData;

//out vec3 tcNormal;
//out vec2 tcTexcoord;


void main( void )
{
    gl_Position =  vec4(vertex, 1);

    outData.normal = normal;
    outData.texcoord = texcoord;
    outData.tangent = tangent;

//    tcNormal = normalize(normal);
//    tcTexcoord = texcoord;
}
