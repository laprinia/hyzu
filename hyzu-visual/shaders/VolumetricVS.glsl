#version 460 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 textureCoord;
out vec2 textCoordonate;

void main() {
    textCoordonate =  (position+1.0)/2;
    gl_Position = vec4(position.xy, 0.0, 1.0);

}