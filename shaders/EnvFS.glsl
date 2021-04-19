#version 460 core
in vec2 outTextureCoord;
in vec3 outNormal;
in vec3 outWorldPosition;
out vec4 fragmentColor;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform sampler2D texture_diffuse1;

void main() {

    float ambientF = 0.2;
    vec3 ambient = ambientF * lightColor;


    vec3 normal = normalize(outNormal);
    vec3 lightDirection = normalize(lightPosition - outWorldPosition);
    float diffuseF = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diffuseF * lightColor;

    float specularF = 0.5;
    vec3 viewDirection = normalize(viewPosition - outWorldPosition);
    vec3 reflectedDirection = reflect(-lightDirection, normal);
    float specularFloat = pow(max(dot(viewDirection, reflectedDirection), 0.0), 32);
    vec3 specular = specularF * specularFloat * lightColor;

    vec3 finalColor = (ambient + diffuse+ specular) * vec3(0.678, 0.678, 0.678);
    //fragmentColor = texture(texture_diffuse1, outTextureCoord);
    fragmentColor = vec4(finalColor, 0.1);

}
