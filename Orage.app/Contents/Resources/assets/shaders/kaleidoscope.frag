
#version 330 core

uniform sampler2D tex0;
uniform sampler2D tex1;
layout (std140) uniform;

uniform kaleidoscope {
    float segments;
    float offset;
    float rotation;
    float scale;
    float x;
    float y;
    bool kaleidoscopeActive;
};

in vec2         vertTexCoord0;
out vec4		oColor;

const float PI = 3.14159265359f;
const float TAU = 2.0 * PI;


void main()
{
    vec4 color = vec4(0.0);
    float kalei  = segments;
    
    if(kaleidoscopeActive){
        vec3 modifierValue = texture(tex1, vertTexCoord0).rgb;
        float modifierV = max(modifierValue.x, max(modifierValue.y, modifierValue.z));
        kalei = kalei * modifierV;
    }
    
    
    if (kalei == 0.0) {
        oColor = texture(tex0, vertTexCoord0);
    } else {
        vec2 centered = vertTexCoord0 - vec2(x, y);
        
        //to polar
        float r = length(centered) * scale;
        float theta = atan(centered.y, centered.x) + rotation * TAU;
        theta = mod(theta, TAU / kalei);
        theta = abs(theta - PI / kalei);
        
        //back to cartesian
        vec2 newCoords = (r * vec2(cos(theta), sin(theta)) + 0.5)  - offset;

        //  Give each cell an index number
        //  according to its position
        float index = 0.0;
        index += step(1., mod(newCoords.x,2.0));
        index += step(1., mod(newCoords.y,2.0))*2.0;
        
        //      |
        //  0   |   1
        //      |
        //--------------
        //      |
        //  2   |   3
        //      |
        
        newCoords = fract(newCoords);
        
        if(index == 1.0 || index == 3.0)
        {
            newCoords.x = 1.0 - newCoords.x;
        }
        if(index == 3.0 || index == 2.0)
        {
            newCoords.y = 1.0 - newCoords.y;
        }
        
        oColor = texture(tex0, fract(newCoords));
    }
}
