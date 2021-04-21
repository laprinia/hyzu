#version 460 core
out vec4 fragmentColor;
in vec2 outTextureCoord;
uniform sampler2D texture1;
void main() {

    fragmentColor = texture(texture1,outTextureCoord);
}

