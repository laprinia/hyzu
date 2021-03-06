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
    vec3 viewPosition;
    vec3 tangentFragmentPosition;
    vec3 tangentViewPosition;
    vec3 tangentLightPosition;
    vec4 fragmentLightSpace;
    vec3 lightNormal;

} vertexData;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightMatrix;

uniform vec3 lightPosition;
uniform vec3 viewPosition;

void main() {
    vertexData.normal = normal;
    vertexData.lightNormal=transpose(inverse(mat3(model))) * normal;
    vertexData.textureCoord = textureCoord;
    vertexData.fragmentPosition = vec3(model* vec4(position, 1.0));
    vertexData.viewPosition = viewPosition;

    mat3 normalMatrix = transpose(inverse(mat3(model)));

    vec3 norm = normalize(normalMatrix * normal);
    vec3 bitan = normalize(normalMatrix * bitangent);
    vec3 tan = cross(bitan, norm);
    mat3 tbnMatrix=transpose(mat3(tan, bitan, norm));

    vertexData.tangentFragmentPosition = tbnMatrix * vertexData.fragmentPosition;
    vertexData.tangentViewPosition = tbnMatrix * viewPosition;
    vertexData.tangentLightPosition = tbnMatrix * lightPosition;
    //shadows
    vertexData.fragmentLightSpace = lightMatrix * vec4(vertexData.fragmentPosition,1.0f);

    gl_Position = projection * view * model * vec4(position, 1.0);

}