#version 400 core

uniform sampler2D texSampler;

in Data {
    vec3 normal;
    vec2 texcoord;
    vec3 position;
} inData;

//in vec3 fNormal;
//in vec2 fTexcoord;

float ka = 0.2;
float kd = 0.9;
float ks = 0.9;
float alpha = 60;

struct DirLight {
    vec3 direction;
    vec3 color;
};

uniform DirLight dirLight;

uniform vec3 eyePosition;

layout(location = 0, index = 0) out vec4 fragColor;

void main( void )
{
    vec3 finalColor;

    finalColor = texture(texSampler, inData.texcoord).xyz;

    /* DEBUG OUTPUT */
//    finalColor = vec3(inData.texcoord.xy, 0);
//    finalColor = inData.position;
//    finalColor = abs(dirLight.direction);

    float Ia = 0.1;
    float Id = clamp(dot(inData.normal, -dirLight.direction), 0, 1);

    vec3 viewDir = eyePosition - inData.position;
    vec3 halfV = normalize(-dirLight.direction + viewDir);

    float Is = 0;
    if (Id > 0) {
        Is = pow(clamp(dot(inData.normal, halfV), 0, 1), alpha);
    }

    vec3 Cfinal = ka*dirLight.color*Ia + kd*dirLight.color*Id + ks*dirLight.color*Is;

    fragColor = vec4( finalColor * Cfinal, 1.0 );
}
