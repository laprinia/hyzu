#version 460 core

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightMatrix;

void main() {

    gl_Position = lightMatrix * model * vec4(position, 1.0);

}