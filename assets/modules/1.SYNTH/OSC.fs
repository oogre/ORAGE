/*{
  "CREDIT": "by vincent evrard",
  "DESCRIPTION": "basic oscillator",
  "ISFVSN": "2",
  "CATEGORIES": [
    "orage"
  ],
  "INPUTS": [
  	{
			"NAME" :	"tex0",
			"TYPE" :	"image"
	},
	{
			"NAME" :	"freq",
			"TYPE" :	"float",
			"DEFAULT" :	1.0,
			"MIN" :	0.0,
			"MAX" :	10.0
	},
	{
			"NAME" :	"sync",
			"TYPE" :	"float",
			"DEFAULT" :	0.0,
			"MIN" :	0.0,
			"MAX" :	1.0
	},
	{
			"NAME" :	"dX",
			"TYPE" :	"float",
			"DEFAULT" :	0.0,
			"MIN" :	0.0,
			"MAX" :	1.0
	},
	{
			"NAME" :	"dY",
			"TYPE" :	"float",
			"DEFAULT" :	0.0,
			"MIN" :	-1.0,
			"MAX" :	1.0
	},
	{
			"NAME" :	"mod",
			"TYPE" :	"float",
			"DEFAULT" :	0.0,
			"MIN" :	-1.0,
			"MAX" :	1.0
	}
  ],
  "OUTPUTS": [
  	{
			"NAME" :	"out0",
			"TYPE" :	"image"
	},{
			"NAME" :	"out1",
			"TYPE" :	"image"
	},{
			"NAME" :	"out2",
			"TYPE" :	"image"
	},{
			"NAME" :	"out3",
			"TYPE" :	"image"
	},{
			"NAME" :	"out4",
			"TYPE" :	"image"
	}
  ]
}*/

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
    return (sin(-0.5*PI + phase * TWO_PI)*0.5+0.5) + height;
}

float nozWave(in float phase, in float height){
    return fract( sin( phase ) * 999.9)+height;
}

float triWave(in float phase, in float height){
    return 1 - abs(((phase)*2.0)-1.0)+ height;
}

float recWave(in float phase, in float height){
	phase = triWave(phase, height);
    if(phase>0.5){
        return 1.0;
    }else{
        return 0.0;
    };
}


void main()
{
  	vec3 modIntensity = IMG_NORM_PIXEL(tex0, isf_FragNormCoord.xy).rgb;
  	float modI = max(modIntensity.r, max(modIntensity.g, modIntensity.b));
	float modFactor = mix(0, modI, _tex0_sample);

	float phase = phaseProcess( isf_FragNormCoord.xy,
                                freq ,
                                sync,
                                (1.0-dX) + mod * modFactor );
    
    out0 = vec4(vec3(sawWave(phase, dY)), 1);
    out1 = vec4(vec3(sinWave(phase, dY)), 1);
    out2 = vec4(vec3(recWave(phase, dY)), 1);
    out3 = vec4(vec3(triWave(phase, dY)), 1);
    out4 = vec4(vec3(nozWave(phase, dY)), 1);
}