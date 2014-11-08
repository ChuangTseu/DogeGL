#version 400 core

uniform sampler2D texSampler;
uniform sampler2D normalMapSampler;

uniform samplerCube cubeMapSampler;

uniform sampler2D shadowMapSampler;

uniform mat4 lightMVP;

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

struct PointLight {
    vec3 position;
    vec3 color;
};

uniform DirLight dirLight;
uniform PointLight pointLight;

uniform vec3 eyePosition;

uniform bool wireframe;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 normalColor;
layout(location = 2) out vec4 texcoordColor;

vec3 blinn_phong_calc_internal(vec3 lightDir, vec3 color, vec3 normal) {
    float Ia = 0.9;
    float Id = clamp(dot(normal, lightDir), 0, 1);

    vec3 viewDir = normalize(eyePosition - inData.position);
    vec3 halfV = normalize(lightDir + viewDir);

    float Is = 0;
    if (Id > 0) {
        Is = pow(clamp(dot(normal, halfV), 0, 1), alpha);
    }

    return ((ka*Ia + kd*Id + ks*Is) * color);
}

vec3 blinn_phong_calc(DirLight light, vec3 normal) {
    vec3 lightDir = normalize(-light.direction);

    return blinn_phong_calc_internal(lightDir, light.color, normal);
}

vec3 blinn_phong_calc(PointLight light, vec3 normal) {
    vec3 lightDir = normalize(light.position - eyePosition);

    return blinn_phong_calc_internal(lightDir, light.color, normal);
}


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

    finalColor = texture(cubeMapSampler, reflect(normalize(eyePosition - inData.position), normal)).xyz;

    vec3 Cfinal = blinn_phong_calc(dirLight, normal) + blinn_phong_calc(pointLight, normal);

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



    vec4 lightSpaceFragPosition = lightMVP * vec4(inData.position, 1);

    vec3 projCoords = lightSpaceFragPosition.xyz / lightSpaceFragPosition.w;

    vec2 shadowMapUVCoords;
    shadowMapUVCoords.x = 0.5 * projCoords.x + 0.5;
    shadowMapUVCoords.y = 0.5 * projCoords.y + 0.5;
    float z = 0.5 * projCoords.z + 0.5;

    float shadowFactor;

    float depth = texture(shadowMapSampler, shadowMapUVCoords).x;
    if (depth < (z - 0.00001))
        shadowFactor = 0.0;
    else
        shadowFactor = 1.0;

    fragColor = fragColor * shadowFactor;

    fragColor = vec4(shadowFactor);


    //    fragColor = texture(cubeMapSampler, reflect(normalize(eyePosition - inData.position), normal));

    normalColor = vec4(normal, 1);
    texcoordColor = vec4(inData.texcoord, 0, 1);
}
