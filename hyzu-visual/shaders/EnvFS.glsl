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
    vec3 position;
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
uniform PointLight point2;
uniform SpotLight spot;
uniform SpotLight spot2;
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
    //float shadow =ComputeShadow(vertexData.fragmentLightSpace);
    float shadow=0.0;
    vec3 result = ambient * color;
    vec3 cViewPosition= normalize(vertexData.viewPosition - vertexData.fragmentPosition);
    result+= ComputeDirLight(directional, tangentNormal, vertexData.tangentViewPosition, vertexData.tangentFragmentPosition, shadow) * color;

    result+= ComputePointLight(point, normal, vertexData.fragmentPosition, cViewPosition) * color;
    result+= ComputePointLight(point2, normal, vertexData.fragmentPosition, cViewPosition) * color;
    result+= ComputeSpotLight(spot, normal, vertexData.fragmentPosition, cViewPosition);
    result+= ComputeSpotLight(spot2, normal, vertexData.fragmentPosition, cViewPosition);

    fragmentColor = vec4(result, 0.1);

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
    shadow /= 12.0;
    return shadow;
}
vec3 ComputeDirLight(DirectionalLight light, vec3 normal, vec3 viewDirection, vec3 fragmentPosition, float shadow) {
    vec3 lightDirection = normalize(-light.direction);

    float diffuseFloat = max(dot(normal, lightDirection), 0.0);

    vec3 viewDirectionC = normalize(viewDirection - fragmentPosition);
    vec3 halfwayDirection=normalize(lightDirection + viewDirectionC);

    float specularFloat = pow(max(dot(normal, halfwayDirection), 0.0), 32);
    vec3 diffuse = light.diffuse * diffuseFloat * (1.0-shadow);
    vec3 specular = light.specular * specularFloat * (1.0-shadow);
    return diffuse + specular;
}

vec3 ComputePointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection) {

    vec3 lightDirection = normalize(light.position-fragmentPosition);
    float diffuseFloat = max(dot(lightDirection, normal), 0.0);

    vec3 viewDirectionC = normalize(viewDirection - fragmentPosition);
    vec3 halfwayDirection = normalize(lightDirection + viewDirectionC);
    float specularFloat = pow(max(dot(normal, halfwayDirection), 0.0), 64);

    float distance = length(light.position-fragmentPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 diffuse = light.diffuse * diffuseFloat;
    vec3 specular = light.specular * specularFloat;

    diffuse *= attenuation;
    specular *= attenuation;

    return (diffuse + specular);

}

vec3 ComputeSpotLight(SpotLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection) {
    vec3 lightDirection = normalize(light.position - fragmentPosition);
    float diffuseFloat = max(dot(normal, lightDirection), 0.0);

    vec3 reflectDirection = reflect(-lightDirection, normal);
    vec3 viewDirectionC = normalize(viewDirection - fragmentPosition);
    vec3 halfwayDirection=normalize(lightDirection + viewDirectionC);
    float specularFloat = pow(max(dot(normal, halfwayDirection), 0.0), 64);

    float distance = length(light.position - fragmentPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    float theta = dot(lightDirection, normalize(light.direction));
    float epsilon = light.cutAngle - light.outterCutAngle;
    float intensity = clamp((theta - light.outterCutAngle) / epsilon, 0.0, 1.0);

    vec3 diffuse = light.diffuse * diffuseFloat;
    vec3 specular = light.specular * specularFloat;

    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (diffuse + specular);
}