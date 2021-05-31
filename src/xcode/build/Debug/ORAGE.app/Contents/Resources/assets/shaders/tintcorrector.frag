
#version 330 core
#include "PhotoshopMathFP.glsl"

uniform sampler2D tex0;
uniform sampler2D tex1;

in vec2         vertTexCoord0;
out vec4		oColor;

layout (std140) uniform;
uniform tintcorrector {
    float hue;
    float sat;
    float light;
    int modifier;
};

in vec3 weights;

void main()
{
    /*
    vec4 color = texture(tex0, vertTexCoord0);
    
    color.rgb = vec3(dot(color.rgb, weights.xyz),
                     dot(color.rgb, weights.zxy),
                     dot(color.rgb, weights.yzx));
    
    
    vec3 adjustment = (color.r + color.g + color.b) / 3.0 - color.rgb;
        if (sat > 0.0) {
            adjustment *= (1.0 - 1.0 / (1.0 - sat));
        } else {
            adjustment *= (-sat);
        }
        color.rgb += adjustment;
        oColor = color;
    */
    vec3 strenght = texture(tex1, vertTexCoord0).rgb;
    float s = max(max(strenght.r, strenght.g), strenght.b);
    if(modifier!=1){
        s = 1;
    }
    vec3 color = RGBToHSL(texture(tex0, vertTexCoord0).rgb);
    color.x = color.x + s * hue;
    color.y = min(max(color.y + s * sat ,0), 1);
    color.z = min(max(color.z + s * light ,0), 1);
    
    oColor = vec4( HSLToRGB(color), 1.0 );
    
    
}

