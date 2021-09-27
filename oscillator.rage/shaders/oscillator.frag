#version 150
float PI = 3.14159265359f;
float TWO_PI = 6.283185306f;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform bool tex1active;

uniform float freq;
uniform float sync;
uniform float dX;
uniform float dY;
uniform float mod;
uniform float sine;
uniform float rect;
uniform float saw;
uniform float tri;
uniform float noz;
uniform float rev;


uniform vec2 resolution;

//in vec2         vertTexCoord0;
out vec4		oColor;

float phaseProcess(in vec2 fs, in float fq, in float sync, in float mod){
    float phase =  mod + (fs.y * (1.0 - sync) + fs.x * sync) * fq;
    return fract(phase);
}

float sawWave(in float phase, in float height){
    return (phase) + height;
}

float sinWave(in float phase, in float height){
    return sin(phase * TWO_PI + ( PI))+step(height, 0)*height+step(0, height)*height*2;//((sin(phase * TWO_PI + (0.5 * PI)) + 1.0)*0.5)+height;
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
    
    vec2 vertTexCoord0 = gl_FragCoord.xy / resolution.xy;
    vec4 prevModValue = texture(tex0, vertTexCoord0);
    
    float phase = phaseProcess(
                               vertTexCoord0,
                               freq * 0.01f*1.6f,
                               sync,
                               dX + mod * (prevModValue.r + prevModValue.g + prevModValue.b));
    
    float value = 0;
    value += saw   * sawWave(phase, dY);
    value += sine  * sinWave(phase, dY);
    value += rect  * recWave(phase, dY);
    value += tri   * triWave(phase, dY);
    value += noz * nozWave(phase, dY);
    value *= rev;
    value = clamp(value, 0.0, 1.0);
    
    if(tex1active){
        oColor = vec4(texture(tex1, vertTexCoord0).rgb * value, 1);
    }
    else{
        oColor = vec4(vec3(value), 1);
    }
}
