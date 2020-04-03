
#version 330 core
#include "PhotoshopMathFP.glsl"

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform float alpha0;
uniform float alpha1;
uniform float blend;

in vec2         vertTexCoord0;
out vec4		oColor;

void main()
{
    if (blend<1.0) {
        oColor = vec4( BlendAdd(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (blend<2.0) {
        oColor = vec4( BlendNormal(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (blend<3.0) {
        oColor = vec4( BlendLighten(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (blend<4.0) {
        oColor = vec4( BlendDarken(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (blend<5.0) {
        oColor = vec4( BlendMultiply(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (blend<6.0) {
        oColor = vec4( BlendAverage(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (blend<7.0) {
        oColor = vec4( BlendSubstract(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (blend<8.0) {
        oColor = vec4( BlendDifference(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (blend<9.0) {
        oColor = vec4( BlendNegation(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (blend<10.0) {
        oColor = vec4( BlendExclusion(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (blend<11.0) {
        oColor = vec4( BlendPhoenix(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (blend<12.0) {
        vec3 A = texture(tex0, vertTexCoord0).rgb * alpha0;
        vec3 B = texture(tex1, vertTexCoord0).rgb * alpha1;
        vec3 C = step(A, vec3(0.5)) * (2.0 * A * B) + (vec3(1.0)-step(A, vec3(0.5))) * (1.0 - 2.0 * (1.0 - A) * (1.0 - B));
        oColor = vec4(C, 1.0 );
    }
    else{
        vec3 A = texture(tex0, vertTexCoord0).rgb * alpha0;
        vec3 B = texture(tex1, vertTexCoord0).rgb * alpha1;
        vec3 C = step(B, vec3(1.0)) * B + (vec3(1.0) - step(B, vec3(1.0))) * min(A * A / (1.0 - B), 1.0) ;
       // oColor = vec4( BlendHardMix(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
}
