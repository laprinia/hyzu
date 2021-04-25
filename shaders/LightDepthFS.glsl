#version 460 core
out vec4 fragmentColor;
in vec2 outTextureCoord;
uniform sampler2D texture1;

uniform float nearPlane;
uniform float farPlane;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}
void main() {

    float depthValue = texture(texture1, outTextureCoord).r;
    fragmentColor = vec4(vec3(LinearizeDepth(depthValue) / farPlane), 1.0);


}