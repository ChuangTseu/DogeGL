#version 400 core

layout(vertices = 3) out;

in Data {
    vec3 normal;
    vec2 texcoord;
} inData[];

out Data {
    vec3 normal;
    vec2 texcoord;
} outData[];

//in vec3 tcNormal[];
//in vec2 tcTexcoord[];

//out vec3 teNormal[];
//out vec2 teTexcoord[];

#define OUTER_LEVEL 20
#define INNER_LEVEL 20

void main(void)
{
        gl_TessLevelOuter[0] = OUTER_LEVEL;
        gl_TessLevelOuter[1] = OUTER_LEVEL;
        gl_TessLevelOuter[2] = OUTER_LEVEL;

        gl_TessLevelInner[0] = INNER_LEVEL;

        gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

        outData[gl_InvocationID].normal = inData[gl_InvocationID].normal;
        outData[gl_InvocationID].texcoord = inData[gl_InvocationID].texcoord;

//        teNormal[gl_InvocationID] = tcNormal[gl_InvocationID];
//        teTexcoord[gl_InvocationID] = tcTexcoord[gl_InvocationID];
}
