
#version 330 core
#include "PhotoshopMathFP.glsl"

uniform sampler2D tex0;
uniform sampler2D tex1;

layout (std140) uniform crossfader {
    float alpha0;
    float alpha1;
    float crossfade;
    float blend;
    int modifierA;
    int modifierB;
};

in vec2         vertTexCoord0;
out vec4		oColor;

void main()
{
    
    int fxID = int(blend);
    if (fxID <= 0 ) {
        oColor = vec4( BlendAdd(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (--fxID <= 0 ) {
        oColor = vec4( BlendLighten(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (--fxID <= 0 ) {
        oColor = vec4( BlendDarken(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (--fxID <= 0 ) {
        oColor = vec4( BlendMultiply(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (--fxID <= 0 ) {
        oColor = vec4( BlendAverage(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (--fxID <= 0 ) {
        oColor = vec4( BlendSubstract(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (--fxID <= 0 ) {
        oColor = vec4( BlendDifference(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (--fxID <= 0 ) {
        oColor = vec4( BlendNegation(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (--fxID <= 0 ) {
        oColor = vec4( BlendExclusion(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (--fxID <= 0 ) {
        oColor = vec4( BlendPhoenix(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else {
        vec3 A = texture(tex0, vertTexCoord0).rgb * alpha0;
        vec3 B = texture(tex1, vertTexCoord0).rgb * alpha1;
        vec3 C = step(A, vec3(0.5)) * (2.0 * A * B) + (vec3(1.0)-step(A, vec3(0.5))) * (1.0 - 2.0 * (1.0 - A) * (1.0 - B));
        oColor = vec4(C, 1.0 );
    }
}
