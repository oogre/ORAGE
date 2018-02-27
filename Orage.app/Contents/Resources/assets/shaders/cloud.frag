#version 330 core

float TWO_PI = 6.283185306f;

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D tex0;
uniform sampler2D tex1;

in vec2		vTexCoord0;
in vec2		vTexCoord1;

uniform vec2 u_resolution;
out vec4        oColor;


layout (location = 0) out vec4 oColor0;
layout (location = 1) out vec4 oColor1;

layout (std140) uniform;
uniform cloud {
    float shiftx;
    float shifty;
    float alpha;
    float redShift;
    float blueShift;
    float greenShift;
    float redLvl;
    float blueLvl;
    float greenLvl;
    float lvl;
    float move;
    float u_time;
    float sizex;
    float sizey;
    float octave;
    float seedXX;
    float seedYX;
    float seedZX;
    float seedXY;
    float seedYY;
    float seedZY;
    int modifierA;
    int modifierB;
};



float random (in vec2 _st) {
    return fract(sin(dot(_st.xy,
                         vec2(12.9898,78.233)))*
                 43758.5453123);
}



// Based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 _st) {
    vec2 i = floor(_st);
    vec2 f = fract(_st);
    
    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));
    
    vec2 u = f * f * (3.0 - 2.0 * f);
    
    return mix(a, b, u.x) +
    (c - a)* u.y * (1.0 - u.x) +
    (d - b) * u.x * u.y;
}

#define NUM_OCTAVES 8

float fbm ( in vec2 _st, in vec2 shift, in float alpha) {
    float v = 0.0;
    float a = 0.5;
    // Rotate to reduce axial bias
    mat2 rot = mat2(cos(alpha * TWO_PI), sin(alpha * TWO_PI),
                    -sin(alpha * TWO_PI), cos(alpha * TWO_PI));
    for (int i = 0; i < octave; ++i) {
        v += a * noise(_st);
        _st = rot * _st * 2.0 + shift;
        a *= 0.5;
    }
    return v;
}

void main() {
    vec3 strenght = texture(tex1, vTexCoord0).rgb;
    float s = max(max(strenght.r, strenght.g), strenght.b);
    if(modifierB!=1){
        s = 1;
    }

    
    vec2 st = vTexCoord0*vec2(sizex, sizey);
    float t = move * u_time * s;
    vec3 i = texture(tex0, vTexCoord0).rgb;
    if(modifierA!=1){
        i = vec3(0, 0, 0);
    }
    
    vec3 q = vec3(0.);
    q.r = fbm( st + vec2(seedXX, seedXY) * t + redShift * i.r, vec2(shiftx, shifty) * 2.0, alpha);
    q.g = fbm( st + vec2(seedYX, seedYY) * t + greenShift * i.g, vec2(shiftx, shifty) * 5.0, alpha);
    q.b = fbm( st + vec2(seedZX, seedZY) * t + blueShift * i.b, vec2(shiftx, shifty) * 14.0, alpha);
    
    vec3 cR = vec3(q.r);
    vec3 cG = vec3(q.g);
    vec3 cB = vec3(q.b);
    
    oColor0 = vec4(vec3(lvl * q), 1);
    oColor1 = vec4(lvl * min(redLvl * cR + greenLvl * cG + blueLvl * cB, vec3(1)), 1);
}