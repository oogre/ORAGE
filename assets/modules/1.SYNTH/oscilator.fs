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
			"NAME" :	"tex1",
			"TYPE" :	"image"
	},
    {
			"NAME" :	"tex2",
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
	},
	{
			"NAME" :	"cos",
			"TYPE" :	"float",
			"DEFAULT" :	0.0,
			"MIN" :	-1.0,
			"MAX" :	1.0
	},
	{
			"NAME" :	"rec",
			"TYPE" :	"float",
			"DEFAULT" :	0.0,
			"MIN" :	-1.0,
			"MAX" :	1.0
	},
	{
			"NAME" :	"saw",
			"TYPE" :	"float",
			"DEFAULT" :	1.0,
			"MIN" :	-1.0,
			"MAX" :	1.0
	},
	{
      		"NAME" :	"tri",
      		"TYPE" : 	"float",
			"DEFAULT" :	0.0,
			"MIN" :	-1.0,
			"MAX" :	1.0
    },
    {
      		"NAME" :	"noz",
      		"TYPE" :	"float",
			"DEFAULT" :	0.0,
			"MIN" :	-1.0,
			"MAX" :	1.0
    },
    {
      		"NAME" :	"rev",
      		"TYPE" :	"float",
			"DEFAULT" :	1.0,
			"MIN" :	0.0,
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
  	vec3 modIntensity = IMG_NORM_PIXEL(tex0, isf_FragNormCoord.xy).rgb;
  	float modI = max(modIntensity.r, max(modIntensity.g, modIntensity.b));
	float modFactor = mix(0, modI, _tex0_sample);

	float phase = phaseProcess( isf_FragNormCoord.xy,
                                freq ,
                                sync,
                                (1.0-dX) + mod * modFactor );
    float value = 0.0;
    value += saw * sawWave(phase, dY);
    value += cos * sinWave(phase, dY);
    value += rec * recWave(phase, dY);
    value += tri * triWave(phase, dY);
    value += noz * nozWave(phase, dY);
    value = clamp(value, 0.0, 1.0);
    value = mix(1.0-value, value,rev);

    vec3 R = IMG_PIXEL(tex0, isf_FragNormCoord.xy).rgb;
    vec3 A = vec3(value);
	vec3 B = mix(vec3(1), IMG_NORM_PIXEL(tex1, isf_FragNormCoord.xy).rgb, vec3(_tex1_sample));
	vec3 C = mix(vec3(0), IMG_NORM_PIXEL(tex2, isf_FragNormCoord.xy).rgb, vec3(_tex2_sample));
	vec3 color = mix(C, B, A);

 	out0 = vec4(color, 1);
}