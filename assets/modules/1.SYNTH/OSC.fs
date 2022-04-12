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
	}
  ]
}*/

float phaseProcess(in vec2 fs, in float fq, in float sync, in float mod){
    float phase =  mod + mix(fs.x, fs.y, sync) * fq;
    return fract(phase);
}
	
float sawWave(in float phase, in float height){
    return (phase) + height;
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
}