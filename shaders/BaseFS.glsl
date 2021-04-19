#version 460 core
in vec3 outTextureCoord;
out vec4 fragmentColor;
uniform sampler2D texture_diffuse1;
void main() {

    fragmentColor = vec4(outTextureCoord,1.0);
}
