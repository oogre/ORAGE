#version 150
float PI = 3.14159265359f;
float TWO_PI = 6.283185306f;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform bool tex1active;

layout (std140) uniform;
uniform oscillator {
    float freq;
    float sync;
    float exp;
    float phaseDx;
    float phaseDy;
    float modifier;
    bool reverse;
    float sine;
    float rect;
    float saw;
    float tri;
    float noise;
};



in vec2         vertTexCoord0;
out vec4		oColor;

float phaseProcess(in vec2 fs, in float fq, in float sync, in float mod){
    float phase =  mod + (fs.y * (1.0 - sync) + fs.x * sync) * fq;
    return fract(phase);
}

float sawWave(in float phase, in float height){
    return (phase) + height;
}

float sinWave(in float phase, in float height){
    return min(1.0,max(0.0, sin(phase * TWO_PI + ( PI))+step(height, 0)*height+step(0, height)*height*2));//((sin(phase * TWO_PI + (0.5 * PI)) + 1.0)*0.5)+height;
}

float nozWave(in float phase, in float height){
    return fract( sin( phase ) * 999.9)+height;
}

float recWave(in float phase, in float height){
    if(sinWave(phase, height)>0.0){
        return 1.0;
    }else{
        return 0.0;
    };
}

float triWave(in float phase, in float height){
    return abs(((phase)*2.0)-1.0)+ height;
}

void main(){
    
    vec4 prevModValue = texture(tex0, vertTexCoord0);
    
    float phase = phaseProcess(
                               vertTexCoord0,
                               freq * 0.01f*1.6f,
                               sync,
                               phaseDx + modifier * (prevModValue.r + prevModValue.g + prevModValue.b));
    
    float value = 0;
    
    if(saw > 0.0){
        value += saw   * sawWave(phase, phaseDy);
    }
    if(sine > 0.0){
        value += sine  * sinWave(phase, phaseDy);
    }
    if(rect > 0.0){
        value += rect  * recWave(phase, phaseDy);
    }
    if(tri > 0.0){
        value += tri   * triWave(phase, phaseDy);
    }
    if(noise > 0.0){
        value += noise * nozWave(phase, phaseDy);
    }
    
    value = clamp(value, 0.0, 1.0);
    
    value = pow(value, exp);
    
    if(reverse == true){
        value = 1.0-value;
    }
    if(tex1active){
        oColor = vec4(texture(tex1, vertTexCoord0).rgb * value, 1);
    }
    else{
        oColor = vec4(vec3(value), 1);
    }
}
