
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
        oColor = vec4( BlendLinearDodge(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else{
        oColor = vec4( BlendLinearBurn(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
}