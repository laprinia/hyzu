#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoord;

out vec2 outTextureCoord;
out vec3 outNormal;
out vec3 outWorldPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    outNormal=normal;
    outTextureCoord = textureCoord;
    outWorldPosition = vec3(model*vec4(position,1.0));
    gl_Position = projection * view * model * vec4(position, 1.0);

}