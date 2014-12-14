#version 400 core

uniform sampler2D texSampler;
uniform sampler2D normalMapSampler;

uniform samplerCube cubeMapSampler;

uniform sampler2DShadow shadowMapSampler;

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

//Material
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float shininess;

float roughness = 0.5;

//vec3 ka = vec3(1, 1, 1);
//vec3 kd = vec3(1, 1, 1);
//vec3 ks = vec3(1, 1, 1);
//float shininess = 64;


layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 normalColor;
layout(location = 2) out vec4 texcoordColor;

vec3 fresnel_schlick(vec3 f0, float LdotH) {
    return f0 + (1 - f0) * pow(1 - LdotH, 5);
}

float ggx_dist() {
    return 0;
}

float beckmann_dist(float m, float NdotH) {
    if (NdotH == 0) return 0; //Tmp, find a better solution not involving any 'if'

    float m_2 = m*m;
    float NdotH_2 = NdotH*NdotH;
    return exp((NdotH_2 - 1)/(m_2 * NdotH_2)) / (m_2*NdotH_2*NdotH_2);
}

float g_term(float NdotH, float NdotV, float VdotH) {
    return min(1, 2*NdotH*NdotV/VdotH);
}

vec3 blinn_phong_calc_internal(vec3 lightDir, vec3 lightColor, vec3 normal) {
    float Id = clamp(dot(normal, lightDir), 0, 1);

    vec3 viewDir = normalize(eyePosition - inData.position);
    vec3 halfV = normalize(lightDir + viewDir);

    float Is = 0;
    if (Id > 0) {
        Is = pow(clamp(dot(normal, halfV), 0, 1), shininess);
    }

    // Replace the 3 colors by light ambiant, diffuse and specular intensity respectively
    float tmpAmbientFactor = 0.2;
    return ka*lightColor*tmpAmbientFactor + (texture(texSampler, inData.texcoord).xyz + ks*Is) * lightColor*Id;
}

vec3 blinn_phong_calc(DirLight light, vec3 normal) {
    vec3 lightDir = normalize(-light.direction);

    return blinn_phong_calc_internal(lightDir, light.color, normal);
}

vec3 blinn_phong_calc(PointLight light, vec3 normal) {
    vec3 lightDir = normalize(light.position - eyePosition);

    return blinn_phong_calc_internal(lightDir, light.color, normal);
}

vec3 cook_torrance_calc_internal(vec3 lightDir, vec3 lightColor, vec3 normal) {
    vec3 viewDir = normalize(eyePosition - inData.position);
    vec3 halfV = normalize(lightDir + viewDir);

    float NdotH = clamp(dot(normal, halfV), 0, 1);
    float NdotV = clamp(dot(normal, viewDir), 0, 1);
    float VdotH = clamp(dot(viewDir, halfV), 0, 1);
    float LdotH = clamp(dot(lightDir, halfV), 0, 1);
    float NdotL = clamp(dot(normal, lightDir), 0, 1);

    float D = beckmann_dist(roughness, NdotH);
    float G = g_term(NdotH, NdotV, VdotH);
    vec3 F = fresnel_schlick(ks, LdotH);

    float Rs_divider = (4*NdotL*NdotV); //Might be zero, douh
    vec3 Rs;
    if (Rs_divider == 0) //Tmp, find a better solution not involving any 'if'
        Rs = vec3(0);
    else
        Rs = (D*F*G) / Rs_divider;

    vec3 diffuse = texture(texSampler, inData.texcoord).xyz;

    return (diffuse + Rs*ks) * lightColor*NdotL;
}

vec3 cook_torrance_calc(DirLight light, vec3 normal) {
    vec3 lightDir = normalize(-light.direction);

    return cook_torrance_calc_internal(lightDir, light.color, normal);
}

vec3 cook_torrance_calc(PointLight light, vec3 normal) {
    vec3 lightDir = normalize(light.position - eyePosition);

    return cook_torrance_calc_internal(lightDir, light.color, normal);
}

const mat4 gracered = mat4(
     0.009098, -0.004780,  0.024033, -0.014947,
    -0.004780, -0.009098, -0.011258,  0.020210,
     0.024033, -0.011258, -0.011570, -0.017383,
    -0.014947,  0.020210, -0.017383,  0.073787
) ;

const mat4 gracegreen = mat4(
    -0.002331, -0.002184,  0.009201, -0.002846,
    -0.002184,  0.002331, -0.009611,  0.017903,
     0.009201, -0.009611, -0.007038, -0.009331,
    -0.002846,  0.017903, -0.009331,  0.041083
) ;

const mat4 graceblue = mat4(
    -0.013032, -0.005248,  0.005970,  0.000483,
    -0.005248,  0.013032, -0.020370,  0.030949,
     0.005970, -0.020370, -0.010948, -0.013784,
     0.000483,  0.030949, -0.013784,  0.051648
) ;

void main( void )
{
    vec3 finalColor;

    vec3 normal = 2.0 * texture(normalMapSampler, inData.texcoord).xyz - vec3(1.0, 1.0, 1.0);

    normal = normalize(normal);

    vec3 worldNormal = normalize(inData.normal);
    vec3 worldTangent = normalize(inData.tangent);
    vec3 worldBitangent = normalize(cross(worldNormal, worldTangent));

    normal = normalize(mat3(worldTangent, worldBitangent, worldNormal) * normal);

//    normal = vec3(gl_FrontFacing ? 1 : -1);

//    normal = normalize(mat3(worldTangent, worldBitangent, worldNormal) * vec3(0,0,1));

//    if (normal.x < 0.997) {
//        discard;
//    }

//    normal = normalize(inData.normal);

//    finalColor = texture(cubeMapSampler, reflect(normalize(eyePosition - inData.position), normal)).xyz;

//    vec3 Cfinal = cook_torrance_calc(dirLight, normal) + cook_torrance_calc(pointLight, normal);
    vec3 Cfinal = blinn_phong_calc(dirLight, normal) + blinn_phong_calc(pointLight, normal);

    fragColor = vec4(Cfinal, 1.0);

//    fragColor = vec4(shininess);


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

//    vec4 lightSpaceFragPosition = lightMVP * vec4(inData.position, 1);

//    vec3 projCoords = lightSpaceFragPosition.xyz / lightSpaceFragPosition.w;

//    vec3 shadowMapUVCoords;
//    shadowMapUVCoords.x = 0.5 * projCoords.x + 0.5;
//    shadowMapUVCoords.y = 0.5 * projCoords.y + 0.5;
//    shadowMapUVCoords.z = 0.5 * projCoords.z + 0.5;
//    float z = 0.5 * projCoords.z + 0.5;

//    shadowMapUVCoords.z = shadowMapUVCoords.z - 0.00005;

//    float shadowFactor;

//    float depth = texture(shadowMapSampler, shadowMapUVCoords);

//    shadowFactor = depth;

//    fragColor = fragColor * shadowFactor;


//    fragColor = texture(cubeMapSampler, reflect(-normalize(eyePosition - inData.position), normal));

    normalColor = vec4(normal, 1);
    texcoordColor = vec4(inData.texcoord, 0, 1);



//    normal = normalize(inData.normal);

//    vec4 n = vec4(normal, 1);

//    float albedo = 6.5;

//    fragColor.x = dot(n, (gracered) * n) * albedo;
//    fragColor.y = dot(n, (gracegreen) * n) * albedo;
//    fragColor.z = dot(n, (graceblue) * n) * albedo;
}
