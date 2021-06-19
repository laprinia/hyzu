#version 460 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 textureCoord;
out vec2 vUv;
out vec2 outTextureCoord;

void main() {
    outTextureCoord=textureCoord;
    vUv = 0.5 * (position+1.0);
    gl_Position = vec4(position.xy, 0.0, 1.0);

}