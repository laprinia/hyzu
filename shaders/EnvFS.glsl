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

in VSDATA {
    vec3 fragmentPosition;
    vec2 textureCoord;
    vec3 normal;
    vec3 worldPosition;

    vec3 tangentFragmentPosition;
    vec3 tangentViewPosition;
    vec3 tangentLightPosition;

} vertexData;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform DirectionalLight directional;
uniform PointLight point;

vec3 ComputeDirLight(DirectionalLight light, vec3 normal, vec3 viewDirection, vec3 fragmentPosition);
vec3 ComputePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {

    vec3 normal = texture(texture_normal1, vertexData.textureCoord).rgb;
    normal= normalize(normal*2.0-1.0);

//    vec3 color = texture(texture_diffuse1, vertexData.textureCoord).rgb;

    vec3 result= ComputeDirLight(directional,normal, vertexData.tangentViewPosition ,vertexData.tangentFragmentPosition);
//    vec3 ambient = 0.1 * color;
//
//    vec3 lightDirection = normalize(vertexData.tangentLightPosition - vertexData.tangentFragmentPosition);
//    float diffuseF = max(dot(lightDirection, normal), 0.0);
//    vec3 diffuse = diffuseF * color;
//
//    float specularF = 0.3;
//    vec3 viewDirection = normalize(vertexData.tangentViewPosition- vertexData.tangentFragmentPosition);
//    vec3 reflectedDirection = reflect(-lightDirection, normal);
//    vec3 halfwayDirection=normalize(lightDirection + viewDirection);
//    float specularFloat = pow(max(dot(normal, halfwayDirection), 0.0), 32);
//    vec3 specular = vec3(specularF) * specularFloat;
//
//    vec3 finalColor = (ambient + diffuse + specular);

    // fragmentColor = texture(texture_normal1, vertexData.textureCoord);
    fragmentColor = vec4(result, 0.1);

}

vec3 ComputeDirLight(DirectionalLight light, vec3 normal, vec3 viewDirection, vec3 fragmentPosition) {
    vec3 lightDirection = normalize(-light.direction);
    float diffuseFloat = max(dot(normal, lightDirection), 0.0);

    vec3 viewDirectionC = normalize(viewDirection- fragmentPosition);
    vec3 halfwayDirection=normalize(lightDirection + viewDirectionC);
    float specularFloat = pow(max(dot(normal, halfwayDirection), 0.0), 32);

    vec3 ambient = light.ambient * texture(texture_diffuse1, vertexData.textureCoord).rgb;
    vec3 diffuse = light.diffuse * diffuseFloat * texture(texture_diffuse1, vertexData.textureCoord).rgb;
    vec3 specular = light.specular * specularFloat * texture(texture_diffuse1, vertexData.textureCoord).rgb;

    return ambient + diffuse + specular;
}

vec3 ComputePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    return vec3(0, 0, 0);
}