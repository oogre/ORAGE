#version 400
	
precision highp float;	
precision highp int;	
	
#define gl_FragColor isf_FragColor
out vec4 gl_FragColor;
uniform int		PASSINDEX;
uniform vec2		RENDERSIZE;
in vec2		isf_FragNormCoord;
uniform float		TIME;
uniform float		TIMEDELTA;
uniform vec4		DATE;
uniform int		FRAMEINDEX;
uniform sampler2D		tex0;
uniform vec4		_tex0_imgRect;
uniform vec2		_tex0_imgSize;
uniform bool		_tex0_flip;
uniform sampler2D		tex1;
uniform vec4		_tex1_imgRect;
uniform vec2		_tex1_imgSize;
uniform bool		_tex1_flip;
uniform float		freq;
uniform float		sync;
uniform float		dX;
uniform float		dY;
uniform float		mod;
uniform float		cos;
uniform float		rec;
uniform float		saw;
uniform float		tri;
uniform float		noz;
uniform float		rev;

vec4 VVSAMPLER_2DBYPIXEL(sampler2D sampler, vec4 samplerImgRect, vec2 samplerImgSize, bool samplerFlip, vec2 loc)	{	
	return (samplerFlip)	
		? texture		(sampler,vec2(((loc.x/samplerImgSize.x*samplerImgRect.z)+samplerImgRect.x), (samplerImgRect.w-(loc.y/samplerImgSize.y*samplerImgRect.w)+samplerImgRect.y)))	
		: texture		(sampler,vec2(((loc.x/samplerImgSize.x*samplerImgRect.z)+samplerImgRect.x), ((loc.y/samplerImgSize.y*samplerImgRect.w)+samplerImgRect.y)));	
}	
vec4 VVSAMPLER_2DBYNORM(sampler2D sampler, vec4 samplerImgRect, vec2 samplerImgSize, bool samplerFlip, vec2 normLoc)	{	
	vec4		returnMe = VVSAMPLER_2DBYPIXEL(		sampler,samplerImgRect,samplerImgSize,samplerFlip,vec2(normLoc.x*samplerImgSize.x, normLoc.y*samplerImgSize.y));	
	return returnMe;	
}	

float PI = 3.14159265359;
float TWO_PI = 6.283185306;

float phaseProcess(in vec2 fs, in float fq, in float sync, in float mod){
    float phase =  mod + mix(fs.x, fs.y, sync) * fq;
    return fract(phase);
}

float sawWave(in float phase, in float height){
    return (phase) + height;
}

float sinWave(in float phase, in float height){
    return sin(phase * TWO_PI)*2.0-1.0;//((sin(phase * TWO_PI + (0.5 * PI)) + 1.0)*0.5)+height;
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

void main()
{
    vec4 prevModValue = VVSAMPLER_2DBYNORM(tex0, _tex0_imgRect, _tex0_imgSize, _tex0_flip, isf_FragNormCoord.xy);
    float modFactor = max(max(prevModValue.r, prevModValue.g), prevModValue.b);
    float phase = phaseProcess( isf_FragNormCoord.xy,
                                freq * 0.016,
                                sync,
                                dX + mod * modFactor );
    float value = 0.0;
    value += saw * sawWave(phase, dY);
    value += cos * sinWave(phase, dY);
    value += rec * recWave(phase, dY);
    value += tri * triWave(phase, dY);
    value += noz * nozWave(phase, dY);
    value = clamp(value, 0.0, 1.0);
    value = mix(1.0-value, value,rev);
    
    //if(false) {
    //    gl_FragColor = vec4(value * VVSAMPLER_2DBYNORM(tex1, _tex1_imgRect, _tex1_imgSize, _tex1_flip, isf_FragNormCoord.xy).rgb, 1);
    //} else {
        gl_FragColor = vec4(vec3(value), 1);
    //}
}
