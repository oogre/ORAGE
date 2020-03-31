
#version 330 core
#include "PhotoshopMathFP.glsl"

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform float alpha0;
uniform float alpha1;


in vec2         vertTexCoord0;
out vec4		oColor;

void main()
{
    oColor = vec4( BlendAdd(texture(tex0, vertTexCoord0).rgb * alpha0, texture(tex1, vertTexCoord0).rgb * alpha1), 1.0 );
}