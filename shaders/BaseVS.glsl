#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 textureCoord;
out vec3 outTextureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    outTextureCoord = position;
    gl_Position = projection * view * model * vec4(position, 1.0);

}