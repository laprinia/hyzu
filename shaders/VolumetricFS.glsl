#version 460 core
out vec4 fragmentColor;
varying vec2 textCoordonate;
uniform sampler2D occTexture;

uniform vec2 screenSpaceSunPosition;
uniform float density;
uniform float weight;
uniform float decay;
uniform float exposure;
uniform int samples;

uniform vec3 directionalColor;
vec3 volumetricLight() {
    vec3 result=vec3(0.0, 0.0, 0.0);
    vec2 deltaTextureCoord=vec2(textCoordonate-screenSpaceSunPosition.xy);
    vec2 textCoord=textCoordonate.xy;
    deltaTextureCoord*=(1.0/float (samples))* density;
    float illuminationDecay=1.0;
    for (int i=0; i < samples; i++){

        textCoord-=deltaTextureCoord;
        vec3 samp= texture2D(occTexture,textCoord).rgb;
        samp*=illuminationDecay* weight;
        result+=samp;
        illuminationDecay*=decay;
    }
    result*=exposure;
    result*=directionalColor;
    return result;
}

void main() {

    fragmentColor = vec4(volumetricLight(), 1.0);
}

