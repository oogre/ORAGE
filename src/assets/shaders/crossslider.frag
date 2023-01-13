
#version 330 core
#include "PhotoshopMathFP.glsl"

uniform sampler2D tex0;
uniform sampler2D tex1;

layout (std140) uniform crossfader {
    float alpha0;
    float alpha1;
    float crossfade;
    float blend;
    int modifierA;
    int modifierB;
};

in vec2         vertTexCoord0;
out vec4		oColor;

void main()
{
     vec2 fs = vertTexCoord0;
    int fxID = int(blend);
    if (fxID <= 0 ) {
        fs.x = mod(0.5+fs.x+(crossfade*0.5), 2.0);
        if(fs.x < 1){
            if(modifierA == 1){
                oColor = vec4(texture(tex0, fs).rgb , 1.0 );
            }else{
                oColor = vec4(vec3(0) , 1.0 );
            }
        }else{
            fs.x -= 1;
            
            if(modifierB == 1){
                oColor = vec4(texture(tex1, fs).rgb , 1.0 );
            }else{
                oColor = vec4(vec3(0) , 1.0 );
            }
        }
    }
    else if(--fxID <= 0){
        fs.y = mod(0.5+fs.y+(1-crossfade*0.5), 2.0);
        if(fs.y < 1){
            
            if(modifierA == 1){
                oColor = vec4(texture(tex0, fs).rgb , 1.0 );
            }else{
                oColor = vec4(vec3(0) , 1.0 );
            }
        }else{
            fs.y -= 1;
            if(modifierB == 1){
                oColor = vec4(texture(tex1, fs).rgb , 1.0 );
            }else{
                oColor = vec4(vec3(0) , 1.0 );
            }
        }
    }
    else if(--fxID <= 0){
        if(fs.x < crossfade * 0.5 + 0.5){
            if(modifierA == 1){
                oColor = vec4(texture(tex0, fs).rgb , 1.0 );
            }else{
                oColor = vec4(vec3(0) , 1.0 );
            }
        }else{
            if(modifierB == 1){
                oColor = vec4(texture(tex1, fs).rgb , 1.0 );
            }else{
                oColor = vec4(vec3(0) , 1.0 );
            }
        }
    }
    else if(--fxID <= 0){
        if(fs.y > crossfade * 0.5 + 0.5){
            if(modifierA == 1){
                oColor = vec4(texture(tex0, fs).rgb , 1.0 );
            }else{
                oColor = vec4(vec3(0) , 1.0 );
            }
        }else{
            if(modifierB == 1){
                oColor = vec4(texture(tex1, fs).rgb , 1.0 );
            }else{
                oColor = vec4(vec3(0) , 1.0 );
            }
        }
    } else if(--fxID <= 0){
        vec2 reFS = fs;
        if(fs.x < 0.5){
            fs.x = mod(0.5+fs.x+(crossfade*0.5), 2.0);
            if(fs.x < 1){
                if(modifierA == 1){
                    oColor = vec4(texture(tex0, fs).rgb , 1.0 );
                }else{
                    oColor = vec4(vec3(0) , 1.0 );
                }
            }else{
                fs.x -= 1;
                
                if(modifierB == 1){
                    oColor = vec4(texture(tex1, reFS).rgb , 1.0 );
                }else{
                    oColor = vec4(vec3(0) , 1.0 );
                }
            }
        }else{
            fs.x = mod(0.5+fs.x+(1-crossfade*0.5), 2.0);
            if(fs.x < 1){
                if(modifierA == 1){
                    oColor = vec4(texture(tex0, fs).rgb , 1.0 );
                }else{
                    oColor = vec4(vec3(0) , 1.0 );
                }
            }else{
                fs.x -= 1;
                
                if(modifierB == 1){
                    oColor = vec4(texture(tex1, reFS).rgb , 1.0 );
                }else{
                    oColor = vec4(vec3(0) , 1.0 );
                }
            }
        }
    } else {
        vec2 reFS = fs;
        if(fs.y < 0.5){
            fs.y = mod(0.5+fs.y+(crossfade*0.5), 2.0);
            if(fs.y < 1){
                if(modifierA == 1){
                    oColor = vec4(texture(tex0, fs).rgb , 1.0 );
                }else{
                    oColor = vec4(vec3(0) , 1.0 );
                }
            }else{
                fs.y -= 1;
                
                if(modifierB == 1){
                    oColor = vec4(texture(tex1, reFS).rgb , 1.0 );
                }else{
                    oColor = vec4(vec3(0) , 1.0 );
                }
            }
        }else{
            fs.y = mod(0.5+fs.y+(1-crossfade*0.5), 2.0);
            if(fs.y < 1){
                if(modifierA == 1){
                    oColor = vec4(texture(tex0, fs).rgb , 1.0 );
                }else{
                    oColor = vec4(vec3(0) , 1.0 );
                }
            }else{
                fs.y -= 1;
                
                if(modifierB == 1){
                    oColor = vec4(texture(tex1, reFS).rgb , 1.0 );
                }else{
                    oColor = vec4(vec3(0) , 1.0 );
                }
            }
        }
    }
}
