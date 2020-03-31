
#version 330 core
#include "PhotoshopMathFP.glsl"
uniform sampler2D tex0;
uniform sampler2D tex1;

in vec2		vTexCoord0;
in vec2		vTexCoord1;
in vec3		vNormal;

layout (location = 0) out vec4 oFragColor0;
layout (location = 1) out vec4 oFragColor1;

void main()
{
    oFragColor0 = vec4( BlendMultiply(texture(tex0, vTexCoord0).rgb, texture(tex1, vTexCoord0).rgb), 1.0 );
    oFragColor1 = vec4( BlendMultiply(BlendDifference( 1 , texture(tex1, vTexCoord0).rgb), texture(tex0, vTexCoord0).rgb), 1.0 );
}