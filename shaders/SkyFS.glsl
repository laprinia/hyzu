#version 460 core
in vec3 outTextureCoord;
out vec4 fragmentColor;
uniform samplerCube skybox;
void main() {

    fragmentColor = texture(skybox,outTextureCoord);

}
