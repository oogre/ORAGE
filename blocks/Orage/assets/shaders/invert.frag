
#version 330 core
//#include "PhotoshopMathFP.glsl"

uniform sampler2D tex0;

in vec2         vertTexCoord0;
out vec4		oColor;

layout (std140) uniform;
uniform invert {
    float red;
    float green;
    float blue;
};


void main()
{

    oColor = vec4( abs(vec3(red, green, blue) - texture(tex0, vertTexCoord0).rgb), 1.0 );
}

