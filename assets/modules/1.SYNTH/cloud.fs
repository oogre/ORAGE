/*{
  "CREDIT": "by vincent evrard",
  "DESCRIPTION": "basic noise2D Cloud generator",
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
			"NAME" :  "_tex0_sample",
			"TYPE" :  "long",
			"DEFAULT": 0
	},
	{
			"NAME" :	"octave",
			"TYPE" :	"float",
			"DEFAULT" :	1.0,
			"MIN" :	1.0,
			"MAX" :	10.0
	},
	{
			"NAME" :	"sizeX",
			"TYPE" :	"float",
			"DEFAULT" :	0.0,
			"MIN" :	0.0,
			"MAX" :	10.0
	},
	{
			"NAME" :	"sizeY",
			"TYPE" :	"float",
			"DEFAULT" :	0.0,
			"MIN" :	0.0,
			"MAX" :	10.0
	},
	{
			"NAME" :	"move",
			"TYPE" :	"float",
			"DEFAULT" :	0.0,
			"MIN" :	-1.0,
			"MAX" :	1.0
	},
	{
			"NAME" :	"redShift",
			"TYPE" :	"float",
			"DEFAULT" :	0.0,
			"MIN" :	-1.0,
			"MAX" :	1.0
	},
	{
			"NAME" :	"greenShift",
			"TYPE" :	"float",
			"DEFAULT" :	0.0,
			"MIN" :	-1.0,
			"MAX" :	1.0
	},
	{
			"NAME" :	"blueShift",
			"TYPE" :	"float",
			"DEFAULT" :	0.0,
			"MIN" :	-1.0,
			"MAX" :	1.0
	},
	{
			"NAME" :	"seedRed",
			"TYPE" :	"float",
			"DEFAULT" :	0.0,
			"MIN" :	-1.0,
			"MAX" :	1.0
	},
	{
			"NAME" :	"seedGreen",
			"TYPE" :	"float",
			"DEFAULT" :	0.0,
			"MIN" :	-1.0,
			"MAX" :	1.0
	},
	{
			"NAME" :	"seedBlue",
			"TYPE" :	"float",
			"DEFAULT" :	0.0,
			"MIN" :	-1.0,
			"MAX" :	1.0
	}
  ],
  "OUTPUTS": [
    {
      "NAME" :  "out0",
      "TYPE" :  "image"
    }
  ]
}*/
#include "shaders/constants.glsl"

float random (in vec2 _st) {
    return fract(sin(dot(_st.xy,
                         vec2(12.9898,78.233)))*
                 43758.5453123);
}

// Based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 _st) {
    vec2 i = floor(_st);
    vec2 f = fract(_st);
    
    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));
    
    vec2 u = f * f * (3.0 - 2.0 * f);
    
    return mix(a, b, u.x) +
    (c - a)* u.y * (1.0 - u.x) +
    (d - b) * u.x * u.y;
}

float fbm ( in vec2 _st) {
    float v = 0.0;
    float a = 0.5;
    vec2 shift = vec2(100.0);
    // Rotate to reduce axial bias
    mat2 rot = mat2(cos(0.5), sin(0.5),
                    -sin(0.5), cos(0.50));
    for (int i = 0; i < octave; ++i) {
        v += a * noise(_st);
        _st = rot * _st * 2.0 + shift;
        a *= 0.5;
    }
    return v;
}

void main(){
	vec2 st = isf_FragNormCoord.xy*vec2(sizeX, sizeY);
    float t = move * TIME;
    vec3 i = IMG_NORM_PIXEL(tex0, isf_FragNormCoord.xy).rgb;
    vec3 q = vec3(0.);

    q.r = fbm( st + vec2(seedRed)   * t + redShift   * i.r);
    q.g = fbm( st + vec2(seedGreen) * t + greenShift * i.g);
    q.b = fbm( st + vec2(seedBlue)  * t + blueShift  * i.b);

    out0 = vec4(vec3(q), 1);
}