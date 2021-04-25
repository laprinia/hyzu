#version 460 core
out vec4 fragmentColor;

struct DirectionalLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 postion;
    vec3 direction;
    float cutAngle;
    float outterCutAngle;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in VSDATA {
    vec3 fragmentPosition;
    vec2 textureCoord;
    vec3 normal;
    vec3 viewPosition;

    vec3 tangentFragmentPosition;
    vec3 tangentViewPosition;
    vec3 tangentLightPosition;
    vec4 fragmentLightSpace;
    vec3 lightNormal;

} vertexData;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D shadowMap;

uniform DirectionalLight directional;
uniform PointLight point;
uniform vec3 viewPosition;

vec3 ComputeDirLight(DirectionalLight light, vec3 normal, vec3 viewDirection, vec3 fragmentPosition, float shadow);
vec3 ComputePointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection);
vec3 ComputeSpotLight(SpotLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection);
float ComputeShadow(vec4 fragmentLightSpace);

void main() {
    if (texture(texture_diffuse1, vertexData.textureCoord).a < 0.3) {
        discard;
    }
    vec3 tangentNormal = texture(texture_normal1, vertexData.textureCoord).rgb;
    tangentNormal = normalize(tangentNormal*2.0-1.0);
    vec3 normal= normalize(vertexData.normal);
    vec3 color=texture(texture_diffuse1, vertexData.textureCoord).rgb;
    vec3 ambient = 0.5 * color;
    float shadow =ComputeShadow(vertexData.fragmentLightSpace);
    vec3 result = ambient * color;
    result+= ComputeDirLight(directional,tangentNormal, vertexData.tangentViewPosition, vertexData.tangentFragmentPosition,shadow) * color;
   // result+= ComputePointLight(point, normal, vertexData.fragmentPosition, viewPosition);

    fragmentColor = vec4(result, 0.1);
    //debug depth map
//
//        vec3 ndcCoords = vertexData.fragmentLightSpace.xyz / vertexData.fragmentLightSpace.w;
//        ndcCoords = ndcCoords * 0.5 + 0.5;
//        float closestDepth = texture(shadowMap, ndcCoords.xy).r;
//        fragmentColor = vec4(ndcCoords.z);
}
float ComputeShadow(vec4 fragmentLightSpace){

    vec3 ndcCoords = fragmentLightSpace.xyz / fragmentLightSpace.w;
    ndcCoords = ndcCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, ndcCoords.xy).r;
    float currentDepth = ndcCoords.z;
    vec3 lightDirection = normalize(directional.position-vertexData.fragmentPosition);
    float bias = max(0.05 * (1.0 - dot(vertexData.lightNormal, lightDirection)), 0.005);
    float shadow =0;
    vec2 texelSize= 1.0/ textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcf = texture(shadowMap, ndcCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcf ? 1.0 : 0.0;
        }
    }
    shadow /= 20.0;
//    if (ndcCoords.z>1.0){
//        shadow=0.0;
//    }
    return shadow;
}
vec3 ComputeDirLight(DirectionalLight light, vec3 normal, vec3 viewDirection, vec3 fragmentPosition,float shadow) {
    vec3 lightDirection = normalize(-light.direction);

    float diffuseFloat = max(dot(normal, lightDirection), 0.0);

    vec3 viewDirectionC = normalize(viewDirection- fragmentPosition);
    vec3 halfwayDirection=normalize(lightDirection + viewDirectionC);

    float specularFloat = pow(max(dot(normal, halfwayDirection), 0.0), 32);
    vec3 diffuse = light.diffuse * diffuseFloat * (1.0-shadow);
    vec3 specular = light.specular * specularFloat * (1.0-shadow);
//        vec3 diffuse = light.diffuse * diffuseFloat;
//        vec3 specular = light.specular * specularFloat;
    return diffuse + specular;
}

vec3 ComputePointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection) {

    vec3 lightDirection = normalize(light.position-fragmentPosition);
    float diffuseFloat = max(dot(lightDirection, normal), 0.0);

    vec3 viewDirectionC = normalize(viewDirection- fragmentPosition);
    vec3 halfwayDirection=normalize(lightDirection + viewDirectionC);
    float specularFloat = pow(max(dot(normal, halfwayDirection), 0.0), 32);

    float distance = length(light.position-fragmentPosition);
    float attenuation = 1.0 / (0.1*distance);

    vec3 diffuse = light.diffuse * diffuseFloat * texture(texture_diffuse1, vertexData.textureCoord).rgb;
    vec3 specular = light.specular * specularFloat * texture(texture_diffuse1, vertexData.textureCoord).rgb;


    diffuse *= attenuation;
    specular *= attenuation;

    return diffuse + specular;

}