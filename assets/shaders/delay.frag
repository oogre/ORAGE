
#version 400 core

#define MAX_TEX 10
#define MAX_IMG_PER_TEX 12

uniform sampler2D tex[MAX_TEX];
uniform sampler2D modifier;
layout (std140) uniform;

uniform delay {
    int bufferLen;
    bool modifierActive;
    int cursor;
    float part;
    float width;
    float height;
};

in vec2         vertTexCoord0;
out vec4		oColor;



void main()
{
    vec4 color = vec4(0.0);
    int c = int(mod(cursor + 1, bufferLen));
    
    if(modifierActive){
        vec3 modifierValue = texture(modifier, vertTexCoord0).rgb;
        float modifierV = max(modifierValue.x, max(modifierValue.y, modifierValue.z));
        c = int(mod(c + int(bufferLen * modifierV), bufferLen));
    }
    
    vec2 pos = vec2 (mod(c, MAX_IMG_PER_TEX) * part, 0);
    //vec2 pos = vec2 (c * part, 0);
    
    oColor = vec4( texture(tex[int(c / MAX_IMG_PER_TEX)], pos + (vec2(width, height) * gl_FragCoord.xy)).rgb, 1);
    //oColor = vec4( texture(tex[0], pos + (vec2(width, height) * gl_FragCoord.xy)).rgb, 1);
}