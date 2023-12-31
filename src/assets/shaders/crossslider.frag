
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

vec4 HSlideAB(vec2 fs, sampler2D A, sampler2D B, float s){
    s = s * 0.5 + 0.5;
    fs.x = mod( fs.x + s, 2.0);
    
    if(fs.x < 1){
        if(modifierA == 1){
            return vec4(texture(A, fs).rgb , 1.0 );
        }
        return vec4(vec3(0) , 1.0 );
    }
    fs.x -= 1;
    if(modifierB == 1){
        return vec4(texture(B, fs).rgb , 1.0 );
    }
    return vec4(vec3(0) , 1.0 );
}

vec4 VSlideAB(vec2 fs, sampler2D A, sampler2D B, float s){
    s = 1 - s * 0.5 + 0.5;
    fs.y = mod( fs.y + s, 2.0);
    
    if(fs.y < 1){
        if(modifierA == 1){
            return vec4(texture(A, fs).rgb , 1.0 );
        }
        return vec4(vec3(0) , 1.0 );
    }
    fs.y -= 1;
    if(modifierB == 1){
        return vec4(texture(B, fs).rgb , 1.0 );
    }
    return vec4(vec3(0) , 1.0 );
}


vec4 HCropeA(vec2 fs, sampler2D A, sampler2D B, float s){
    s = 1 - (s * 0.5 + 0.5);
    
    if(fs.x < s){
        if(modifierA == 1){
            return vec4(texture(A, fs).rgb , 1.0 );
        }
        return vec4(vec3(0) , 1.0 );
    }
    if(modifierB == 1){
        return  vec4(texture(B, fs).rgb , 1.0 );
    }
    return vec4(vec3(0) , 1.0 );
}


vec4 VCropeA(vec2 fs, sampler2D A, sampler2D B, float s){
    s = s * 0.5 + 0.5;
    
    if(fs.y > s){
        if(modifierA == 1){
            return vec4(texture(A, fs).rgb , 1.0 );
        }
        return vec4(vec3(0) , 1.0 );
    }
    if(modifierB == 1){
        return vec4(texture(B, fs).rgb , 1.0 );
    }
    return vec4(vec3(0) , 1.0 );
}

vec4 LRHSlideA(vec2 fs, sampler2D A, sampler2D B, float s){
    vec2 reFS = fs;
    if(fs.x < 0.5){
        s = 0.5 + s * 0.5;
        
        fs.x = mod( fs.x + s, 2.0 );
        
        if(fs.x < 1){
            if(modifierA == 1){
                return vec4(texture(A, fs).rgb , 1.0 );
            }
            return vec4(vec3(0) , 1.0 );
        }
        fs.x -= 1;
            
        if(modifierB == 1){
            return vec4(texture(B, reFS).rgb , 1.0 );
        }
        return vec4(vec3(0) , 1.0 );
    }
    s = 1 - s * 0.5 + 0.5;
    
    fs.x = mod( fs.x + s, 2.0 );
    if(fs.x < 1){
        if(modifierA == 1){
            return vec4(texture(A, fs).rgb , 1.0 );
        }
        return vec4(vec3(0) , 1.0 );
    }
    fs.x -= 1;
    if(modifierB == 1){
        return vec4(texture(B, reFS).rgb , 1.0 );
    }
    return vec4(vec3(0) , 1.0 );
}

vec4 TBVSlideA(vec2 fs, sampler2D A, sampler2D B, float s){
    vec2 reFS = fs;
    if(fs.y < 0.5){
        s = 0.5 + s * 0.5;
        
        fs.y = mod( fs.y + s , 2.0);
        if(fs.y < 1){
            if(modifierA == 1){
                return vec4(texture(A, fs).rgb , 1.0 );
            }
            return vec4(vec3(0) , 1.0 );
        }
        fs.y -= 1;
            
        if(modifierB == 1){
            return vec4(texture(B, reFS).rgb , 1.0 );
        }
        return vec4(vec3(0) , 1.0 );
    }
    s = 1 - s * 0.5 + 0.5;
    fs.y = mod( fs.y + s , 2.0);
    if(fs.y < 1){
        if(modifierA == 1){
            return vec4(texture(A, fs).rgb , 1.0 );
        }
        return vec4(vec3(0) , 1.0 );
    }
    fs.y -= 1;
            
    if(modifierB == 1){
        return vec4(texture(B, reFS).rgb , 1.0 );
    }
    return vec4(vec3(0) , 1.0 );
}

void main()
{
    vec2 fs = vertTexCoord0;
    
//    bool xRange = fs.x >= 0 && fs.x < 1;
//    bool yRange = fs.y >= 0 && fs.y < 1;
//    if( xRange && yRange ){
//        if(modifierA == 1){
//            return vec4(texture(tex0, fs).rgb , 1.0 );
//        }
//        return vec4(vec3(0) , 1.0 );
//    }
//    if(!xRange){
//        fs.x += 10;
//        fs.x = mod(fs.x, 1);
//    }
//    if(!yRange){
//        fs.y += 10;
//        fs.y = mod(fs.x, 1);
//    }
//    if(modifierB == 1){
//        return vec4(texture(tex1, reFS).rgb , 1.0 );
//    }
//    return vec4(vec3(0) , 1.0 );
    
    switch(int(blend)){
        case 0 :
            oColor = HSlideAB(vertTexCoord0, tex0, tex1, crossfade);
        break;
        case 1 :
            oColor = VSlideAB(vertTexCoord0, tex0, tex1, crossfade);
        break;
        case 2 :
            oColor = HCropeA(vertTexCoord0, tex0, tex1, crossfade);
        break;
        case 3 :
            oColor = VCropeA(vertTexCoord0, tex0, tex1, crossfade);
        break;
        case 4 :
            oColor = LRHSlideA(vertTexCoord0, tex0, tex1, crossfade);
        break;
        case 5 :
            oColor = TBVSlideA(vertTexCoord0, tex0, tex1, crossfade);
        break;
        default :
            oColor = vec4(1.0);
        break;
    }
}
