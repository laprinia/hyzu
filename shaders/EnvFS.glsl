#version 460 core
out vec4 fragmentColor;

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

void main() {

    //vec3 normal = normalize(vertexData.normal);
    vec3 normal = texture(texture_normal1, vertexData.textureCoord).rgb;
    normal= normalize(normal*2.0-1.0);

    vec3 color = texture(texture_diffuse1, vertexData.textureCoord).rgb;

    vec3 ambient = 0.1 * color;

    vec3 lightDirection = normalize(vertexData.tangentLightPosition - vertexData.tangentFragmentPosition);
    float diffuseF = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = diffuseF * color;

    float specularF = 0.2;
    vec3 viewDirection = normalize(vertexData.tangentViewPosition- vertexData.tangentFragmentPosition);
    vec3 reflectedDirection = reflect(-lightDirection, normal);
    vec3 halfwayDirection=normalize(lightDirection + viewDirection);
    float specularFloat = pow(max(dot(viewDirection, halfwayDirection), 0.0), 32);
    vec3 specular = vec3(specularF) * specularFloat;

    vec3 finalColor = (ambient + diffuse + specular);
   // fragmentColor = texture(texture_normal1, vertexData.textureCoord);
    fragmentColor = vec4(finalColor, 0.1);

}
