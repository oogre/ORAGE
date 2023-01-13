
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
    
    if (blend<1.0) {
        oColor = vec4( BlendAdd(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (blend<2.0) {
        oColor = vec4( BlendLighten(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (blend<3.0) {
        oColor = vec4( BlendDarken(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (blend<4.0) {
        oColor = vec4( BlendMultiply(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (blend<5.0) {
        oColor = vec4( BlendAverage(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (blend<6.0) {
        oColor = vec4( BlendSubstract(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (blend<7.0) {
        oColor = vec4( BlendDifference(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (blend<8.0) {
        oColor = vec4( BlendNegation(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (blend<9.0) {
        oColor = vec4( BlendExclusion(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (blend<10.0) {
        oColor = vec4( BlendPhoenix(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
    }
    else if (blend<11.0) {
        vec3 A = texture(tex0, vertTexCoord0).rgb * alpha0;
        vec3 B = texture(tex1, vertTexCoord0).rgb * alpha1;
        vec3 C = step(A, vec3(0.5)) * (2.0 * A * B) + (vec3(1.0)-step(A, vec3(0.5))) * (1.0 - 2.0 * (1.0 - A) * (1.0 - B));
        oColor = vec4(C, 1.0 );
    }
    else if (blend<12.0) {
        vec2 fs = vertTexCoord0;
        fs.x = mod(0.5+fs.x+(crossfade*0.5), 2.0);
        if(fs.x < 1){
            if(modifierA == 1){
                oColor = vec4(texture(tex0, fs).rgb , 1.0 );
            }else{
                oColor = vec4(vec3(0) , 1.0 );
            }
        }else{
            fs.x -= 1;
            
            if(modifierB == 1){
                oColor = vec4(texture(tex1, fs).rgb , 1.0 );
            }else{
                oColor = vec4(vec3(0) , 1.0 );
            }
        }
    }
    else if (blend<13.0) {
        vec2 fs = vertTexCoord0;
        fs.y = mod(0.5+fs.y+(1-crossfade*0.5), 2.0);
        if(fs.y < 1){
            
            if(modifierA == 1){
                oColor = vec4(texture(tex0, fs).rgb , 1.0 );
            }else{
                oColor = vec4(vec3(0) , 1.0 );
            }
        }else{
            fs.y -= 1;
            if(modifierB == 1){
                oColor = vec4(texture(tex1, fs).rgb , 1.0 );
            }else{
                oColor = vec4(vec3(0) , 1.0 );
            }
        }
    }
    else if (blend<14.0) {
        vec2 fs = vertTexCoord0;
        
        if(fs.x < crossfade * 0.5 + 0.5){
            if(modifierA == 1){
                oColor = vec4(texture(tex0, fs).rgb , 1.0 );
            }else{
                oColor = vec4(vec3(0) , 1.0 );
            }
        }else{
            if(modifierB == 1){
                oColor = vec4(texture(tex1, fs).rgb , 1.0 );
            }else{
                oColor = vec4(vec3(0) , 1.0 );
            }
        }
    }
    else {
        vec2 fs = vertTexCoord0;
        if(fs.y > crossfade * 0.5 + 0.5){
            if(modifierA == 1){
                oColor = vec4(texture(tex0, fs).rgb , 1.0 );
            }else{
                oColor = vec4(vec3(0) , 1.0 );
            }
        }else{
            if(modifierB == 1){
                oColor = vec4(texture(tex1, fs).rgb , 1.0 );
            }else{
                oColor = vec4(vec3(0) , 1.0 );
            }
        }
    }
}
