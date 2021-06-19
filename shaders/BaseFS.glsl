#version 460 core
in vec2 outTextureCoord;
out vec4 fragmentColor;
uniform sampler2D texture_diffuse1;

uniform vec3 solidColor;
void main() {

    fragmentColor = vec4(solidColor,1.0);

}
