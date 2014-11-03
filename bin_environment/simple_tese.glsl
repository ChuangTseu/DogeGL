#version 400 core

layout(triangles, equal_spacing, cw) in;

in Data {
    vec3 normal;
    vec2 texcoord;
    vec3 tangent;
} inData[];

out Data {
    vec3 normal;
    vec2 texcoord;
    vec3 tangent;

    vec3 position;
} outData;

//in vec3 teNormal[];
//in vec2 teTexcoord[];

//out vec3 fNormal;
//out vec2 fTexcoord;

// Uniform

uniform mat4 MVP;
uniform mat4 world;

uniform sampler2D dispMapSampler;

uniform float userDisplacementFactor;

void main()
{
        gl_Position.xyzw =  gl_in[0].gl_Position.xyzw * gl_TessCoord.x +
                            gl_in[1].gl_Position.xyzw * gl_TessCoord.y +
                            gl_in[2].gl_Position.xyzw * gl_TessCoord.z;

        outData.normal = inData[0].normal * gl_TessCoord.x + inData[1].normal * gl_TessCoord.y + inData[2].normal * gl_TessCoord.z;
        outData.texcoord = inData[0].texcoord * gl_TessCoord.x + inData[1].texcoord * gl_TessCoord.y + inData[2].texcoord * gl_TessCoord.z;
        outData.tangent = inData[0].tangent * gl_TessCoord.x + inData[1].tangent * gl_TessCoord.y + inData[2].tangent * gl_TessCoord.z;

//        fNormal = teNormal[0] * gl_TessCoord.x + teNormal[1] * gl_TessCoord.y + teNormal[2] * gl_TessCoord.z;
//        fTexcoord = teTexcoord[0] * gl_TessCoord.x + teTexcoord[1] * gl_TessCoord.y + teTexcoord[2] * gl_TessCoord.z;

        vec3 rgb_disp = texture(dispMapSampler, outData.texcoord).xyz;

        float disp_factor = 0.30*rgb_disp.x + 0.59*rgb_disp.y + 0.11*rgb_disp.z;

        gl_Position.xyz = gl_Position.xyz + normalize(outData.normal)*disp_factor*userDisplacementFactor;


        /* Wow, such sphere */
//        gl_Position.xyz = 3*normalize(gl_Position.xyz);

        outData.normal = normalize((world * vec4(normalize(outData.normal), 0)).xyz);
        outData.tangent = normalize((world * vec4(normalize(outData.tangent), 0)).xyz);
        outData.position = (world * vec4(gl_Position.xyz, 1)).xyz;

        gl_Position = MVP * gl_Position;



}
