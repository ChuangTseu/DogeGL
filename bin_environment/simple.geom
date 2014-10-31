#version 400 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in Data {
    vec3 normal;
    vec2 texcoord;
    vec3 tangent;

    vec3 position;
} inData[];

out Data {
    vec3 normal;
    vec2 texcoord;
    vec3 tangent;

    vec3 position;
    vec3 color;
} outData;


void main()
{
        gl_Position = gl_in[0].gl_Position;

        outData.normal = inData[0].normal;
        outData.texcoord = inData[0].texcoord;
        outData.tangent = inData[0].tangent;

        outData.position = inData[0].position;
        outData.color = vec3(1, 0, 0);
        gl_PrimitiveID = gl_PrimitiveIDIn;

        EmitVertex();

        gl_Position = gl_in[1].gl_Position;

        outData.normal = inData[1].normal;
        outData.texcoord = inData[1].texcoord;
        outData.tangent = inData[1].tangent;

        outData.position = inData[1].position;
        outData.color = vec3(0, 1, 0);
        gl_PrimitiveID = gl_PrimitiveIDIn;

        EmitVertex();

        gl_Position = gl_in[2].gl_Position;

        outData.normal = inData[2].normal;
        outData.texcoord = inData[2].texcoord;
        outData.tangent = inData[2].tangent;

        outData.position = inData[2].position;
        outData.color = vec3(0, 0, 1);
        gl_PrimitiveID = gl_PrimitiveIDIn;

        EmitVertex();

}
