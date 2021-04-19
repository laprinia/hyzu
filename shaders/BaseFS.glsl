#version 460 core
in vec2 outTextureCoord;
out vec4 fragmentColor;
uniform sampler2D texture_diffuse1;
void main() {

    fragmentColor = vec4(1.0,1.0,1.0,1.0);

}
