
#version 330 core
#include "PhotoshopMathFP.glsl"

uniform sampler2D tex0;

in vec2         vertTexCoord0;
out vec4		oColor;

layout (std140) uniform;
uniform coloradjustement {
    float bri;
    float sat;
    float con;
};


void main()
{
    ;
    oColor = vec4( ContrastSaturationBrightness(texture(tex0, vertTexCoord0).rgb, bri, sat, con), 1.0 );
}

