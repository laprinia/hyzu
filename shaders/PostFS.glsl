#version 460 core
out vec4 fragmentColor;
in vec2 outTextureCoord;
uniform sampler2D texture1;
uniform float lightExposure;
uniform float time;

float noiseStrength = 1.3;


float noise (vec2 point) {
    vec2 K1 =vec2( 23.14069263277926,
    2.665144142690225);
    return fract(cos(dot(point,K1))* 13345.6789);
}
void main() {

    const float gammaValue = 2.2;
    vec3 base = texture(texture1, outTextureCoord).rgb;
    vec3 result = vec3(1.0) - exp(-base * lightExposure);
    result = pow(result, vec3(1.0/gammaValue));
    vec2 random=outTextureCoord;
    random.y*=noise(vec2(random.y,time));
    result+=noise(random)*0.15 * noiseStrength;
    fragmentColor = vec4(result, 1.0);
}

