#version 460 core

layout (location = 0) in vec3 position;

out vec3 outTextureCoord;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main() {
    outTextureCoord = position;
    vec4 worldPosition= projection * view *model * vec4(position, 1.0);
    gl_Position=worldPosition.xyww;
}