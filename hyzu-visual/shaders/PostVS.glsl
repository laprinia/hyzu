#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoord;

out vec2 outTextureCoord;

void main() {
    outTextureCoord=textureCoord;
    gl_Position= vec4(position.x,position.y,0, 1.0);

}