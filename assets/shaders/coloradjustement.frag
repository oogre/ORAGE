
#version 330 core
#include "PhotoshopMathFP.glsl"

uniform sampler2D tex0;
uniform sampler2D tex1;

in vec2         vertTexCoord0;
out vec4		oColor;

layout (std140) uniform;
uniform coloradjustement {
    float bri;
    float sat;
    float con;
    float red;
    float green;
    float blue;
    int modifier;
};


void main()
{
    vec3 strenght = texture(tex1, vertTexCoord0).rgb;
    float s = max(max(strenght.r, strenght.g), strenght.b);
    if(modifier!=1){
        s = 1;
    }
    
    vec3 fader = step(vec3(red, green, blue), vec3(0)) * vec3(red, green, blue);
    vec3 nega =  step(vec3(0), vec3(red, green, blue)) * vec3(red, green, blue);
    vec3 color = texture(tex0, vertTexCoord0).rgb;
    vec3 corrected = ContrastSaturationBrightness(abs(color - nega) + (color * fader), bri, sat, con);
    
    oColor = vec4( s * corrected + (1 - s)*color, 1.0 );
    
    
    

}

