#version 400 core

uniform sampler2D texSampler;

in Data {
    vec3 normal;
    vec2 texcoord;
    vec3 position;
} inData;

//in vec3 fNormal;
//in vec2 fTexcoord;

layout(location = 0, index = 0) out vec4 fragColor;

void main( void )
{
    vec3 finalColor = abs(max(vec3(-0.2f), normalize(inData.normal)));

    finalColor = texture(texSampler, inData.texcoord).xyz;

    /* DEBUG OUTPUT */
//    finalColor = vec3(inData.texcoord.xy, 0);
//    finalColor = inData.position;

    fragColor = vec4( finalColor, 1.0 );
}
