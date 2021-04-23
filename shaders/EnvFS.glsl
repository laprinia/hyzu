#version 460 core
out vec4 fragmentColor;

struct DirectionalLight {
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
    vec3 worldPosition;

    vec3 tangentFragmentPosition;
    vec3 tangentViewPosition;
    vec3 tangentLightPosition;

} vertexData;



uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

uniform DirectionalLight directional;
uniform PointLight point;
uniform vec3 viewPosition;

vec3 ComputeDirLight(DirectionalLight light, vec3 normal, vec3 viewDirection, vec3 fragmentPosition);
vec3 ComputePointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection);
vec3 ComputeSpotLight(SpotLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection);

void main() {
    if (texture(texture_diffuse1, vertexData.textureCoord).a < 0.3) {
        discard;
    }
    vec3 normal = texture(texture_normal1, vertexData.textureCoord).rgb;
    normal = normalize(normal*2.0-1.0);
    vec3 color=texture(texture_diffuse1, vertexData.textureCoord).rgb;
    vec3 ambient = 0.05 * color;
    vec3 result = ambient;
    result+= ComputeDirLight(directional, normal, vertexData.tangentViewPosition, vertexData.tangentFragmentPosition);
   // result+= ComputePointLight(point, normal, vertexData.fragmentPosition, viewPosition);

    fragmentColor = vec4(result, 0.1);
}

vec3 ComputeDirLight(DirectionalLight light, vec3 normal, vec3 viewDirection, vec3 fragmentPosition) {
    vec3 lightDirection = normalize(-light.direction);
    float diffuseFloat = max(dot(lightDirection, normal), 0.0);
    vec3 viewDirectionC = normalize(viewDirection- fragmentPosition);
    vec3 halfwayDirection=normalize(lightDirection + viewDirectionC);
    float specularFloat = pow(max(dot(normal, halfwayDirection), 0.0), 32);
    vec3 diffuse = light.diffuse * diffuseFloat * texture(texture_diffuse1, vertexData.textureCoord).rgb;
    vec3 specular = light.specular * specularFloat * texture(texture_diffuse1, vertexData.textureCoord).rgb;

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

    return  diffuse + specular;

}