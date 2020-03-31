
#version 330 core
//#include "PhotoshopMathFP.glsl"

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;

in vec2         vertTexCoord0;
out vec4		oColor;

layout (std140) uniform;
uniform trail {
    float amount;
    uniform float black;
    uniform float white;
};


void main()
{
    oColor = vec4(mix(texture(tex0, vertTexCoord0).rgb, texture(tex1, vertTexCoord0).rgb, amount *  smoothstep(black, white, texture(tex2, vertTexCoord0).rgb)), 1.0);
}

