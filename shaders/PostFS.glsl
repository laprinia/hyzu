#version 460 core
out vec4 fragmentColor;
in vec2 outTextureCoord;
uniform sampler2D texture1;
uniform float lightExposure;
void main() {

    const float gammaValue = 2.2;
    vec3 base = texture(texture1, outTextureCoord).rgb;
    vec3 result = vec3(1.0) - exp(-base * lightExposure);
    result = pow(result, vec3(1.0/gammaValue));
    fragmentColor = vec4(result, 1.0);

}

