/*{
  "CREDIT": "by vincent evrard",
  "DESCRIPTION": "basic oscillator",
  "ISFVSN": "2",
  "CATEGORIES": [
    "orage"
  ],
  "INPUTS": [
  	{
			"NAME" :	"gradiant",
			"TYPE" :	"image"
	},
    {
			"NAME" :	"color1",// background
			"TYPE" :	"image"
	},
    {
			"NAME" :	"color2",// foreground
			"TYPE" :	"image"
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

#include "shaders/constants.glsl"


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
	
  	vec3 modIntensity = IMG_NORM_PIXEL(gradiant, isf_FragNormCoord.xy).rgb;
    float phase = max(max(modIntensity.r, modIntensity.g), modIntensity.b);
    float value = 0.0;
    value += saw * sawWave(phase, 0.0);
    value += cos * sinWave(phase, 0.0);
    value += rec * recWave(phase, 0.0);
    value += tri * triWave(phase, 0.0);
    value += noz * nozWave(phase, 0.0);
    value = clamp(value, -1.0, 1.0);
    value = value * 0.5 + 0.5;
    value = mix(1.0-value, value,rev);

    vec3 R = IMG_PIXEL(gradiant, isf_FragNormCoord.xy).rgb;
    vec3 A = vec3(value);
	vec3 B = mix(vec3(1), IMG_NORM_PIXEL(color1, isf_FragNormCoord.xy).rgb, vec3(_color1_sample));
	vec3 C = mix(vec3(0), IMG_NORM_PIXEL(color2, isf_FragNormCoord.xy).rgb, vec3(_color2_sample));
	vec3 color = mix(C, B, A);

 	out0 = vec4(color, 1);
}