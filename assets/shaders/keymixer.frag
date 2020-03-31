
#version 330 core
#include "PhotoshopMathFP.glsl"

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
//uniform float alpha0;
//uniform float alpha1;
//uniform float alpha2;


in vec2         vertTexCoord0;
out vec4		oColor;

void main()
{
    
   // oFragColor0 = vec4( BlendMultiply(texture(tex0, vTexCoord0).rgb, texture(tex1, vTexCoord0).rgb), 1.0 );
    vec3 extract = BlendMultiply(BlendDifference( 1 , texture(tex1, vertTexCoord0).rgb), texture(tex0, vertTexCoord0).rgb);
    vec4 extr = Desaturate (extract, 1);
    oColor = vec4( mix(extr.r * extract.rgb, texture(tex2, vertTexCoord0).rgb, extr.rgb), 1.0 );
}

