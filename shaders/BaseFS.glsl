#version 460 core
in vec4 vertexColor;
out vec4 fragmentColor;
uniform vec4 uniformColor;
void main() {

    fragmentColor = vertexColor;
}
