#version 400 core

uniform sampler2D texSampler;
uniform sampler2D normalMapSampler;

in Data {
    vec3 normal;
    vec2 texcoord;
    vec3 tangent;

    vec3 position;
    vec3 color;
} inData;

//in vec3 fNormal;
//in vec2 fTexcoord;

float ka = 0.2;
float kd = 0.9;
float ks = 2;
float alpha = 512;

struct DirLight {
    vec3 direction;
    vec3 color;
};

uniform DirLight dirLight;

uniform vec3 eyePosition;

uniform bool wireframe;

layout(location = 0, index = 0) out vec4 fragColor;

void main( void )
{
    vec3 finalColor;

    vec3 normal = 2.0 * texture(normalMapSampler, inData.texcoord).xyz - vec3(1.0, 1.0, 1.0);

//    normal = normalize(normal);

    vec3 worldNormal = normalize(inData.normal);
    vec3 worldTangent = normalize(inData.tangent);
    vec3 worldBitangent = normalize(cross(worldNormal, worldTangent));

    normal = normalize(mat3(worldTangent, worldBitangent, worldNormal) * normal);

//    normal = normalize(mat3(worldTangent, worldBitangent, worldNormal) * vec3(0,0,1));

//    if (normal.x < 0.997) {
//        discard;
//    }

//    normal = worldBitangent;

//    normal = inData.normal;

    finalColor = texture(texSampler, inData.texcoord).xyz;

    float Ia = 0.9;
    float Id = clamp(dot(normal, -dirLight.direction), 0, 1);

    vec3 viewDir = normalize(eyePosition - inData.position);
    vec3 halfV = normalize(normalize(-dirLight.direction) + viewDir);

    float Is = 0;
    if (Id > 0) {
        Is = pow(clamp(dot(normal, halfV), 0, 1), alpha);
    }

    vec3 Cfinal = ka*dirLight.color*Ia + kd*dirLight.color*Id + ks*dirLight.color*Is;

    fragColor = vec4( finalColor * Cfinal, 1.0 );

    /* DEBUG OUTPUT */
    /* WIREFRAME FAIT MAISON */
    if (wireframe) {
        float threshold = 0.01;
        if(inData.color.x >= threshold && inData.color.y >= threshold && inData.color.z >= threshold) {
            discard;
        }
        else {
    //        fragColor = vec4( inData.color, 1.0 );
        }
    }



//    fragColor = vec4(inData.texcoord, 0, 1);
//    fragColor = vec4(texture(normalMapSampler, inData.texcoord).xyz, 1);

//    fragColor = vec4( normal, 1.0 );
//    fragColor = vec4(vec3(0,0,1), 1);
//    fragColor = vec4( cross(tangent, normal), 1.0 );



}
