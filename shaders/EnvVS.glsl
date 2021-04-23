#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;


out VSDATA {
    vec3 fragmentPosition;
    vec2 textureCoord;
    vec3 normal;
    vec3 worldPosition;
    vec3 tangentFragmentPosition;
    vec3 tangentViewPosition;
    vec3 tangentLightPosition;
    vec4 fragmentLightSpace;

} vertexData;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightMatrix;

uniform vec3 lightPosition;
uniform vec3 viewPosition;

void main() {
    vertexData.normal = normal;
    vertexData.textureCoord = textureCoord;
    vertexData.worldPosition = vec3(model*vec4(position, 1.0));
    vertexData.fragmentPosition = vec3(model* vec4(position, 1.0));

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 tan = normalize(normalMatrix * tangent);
    vec3 norm = normalize(normalMatrix * normal);
    vec3 bitan = normalize(normalMatrix * bitangent);

    tan = normalize(tan-dot(tan, norm) * norm);
    mat3 tbnMatrix=transpose(mat3(tan, bitan, norm));

    vertexData.tangentFragmentPosition = tbnMatrix * vertexData.fragmentPosition;
    vertexData.tangentViewPosition = tbnMatrix * viewPosition;
    vertexData.tangentLightPosition = tbnMatrix * lightPosition;
    //shadows
    vertexData.fragmentLightSpace = lightMatrix * vec4(vertexData.fragmentPosition,1.0f);

    gl_Position = projection * view * model * vec4(position, 1.0);

}