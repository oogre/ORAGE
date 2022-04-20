/*{
  "CREDIT": "by vincent evrard",
  "DESCRIPTION": "basic YPbPr color generator",
  "ISFVSN": "2",
  "CATEGORIES": [
    "orage"
  ],
  "INPUTS": [
  	{
      "NAME" :  "TEX_A",
      "TYPE" :  "image"
    },
    {
      "NAME" :  "TEX_B",
      "TYPE" :  "image"
    },
    {
      "NAME" :  "TEX_MIX",
      "TYPE" :  "image"
    },
    {
      "NAME" :  "blend",
      "TYPE" :  "float",
      "DEFAULT" : 0.0,
      "MIN" : 0.0,
      "MAX" : 12.0
    },
    {
      "NAME" :  "amount",
      "TYPE" :  "float",
      "DEFAULT" : 0.0,
      "MIN" : -1.0,
      "MAX" : 1.0
    },
    {
      "NAME" :  "black",
      "TYPE" :  "float",
      "DEFAULT" : 0.0,
      "MIN" : 0.0,
      "MAX" : 1.0
    },
    {
      "NAME" :  "white",
      "TYPE" :  "float",
      "DEFAULT" : 1.0,
      "MIN" : 0.0,
      "MAX" : 1.0
    }
  ],
  "OUTPUTS": [
    {
      "NAME" :  "out0",
      "TYPE" :  "image"
    }
  ]
}*/

#include "shaders/PhotoshopMathFP.glsl"

void main()
{
  vec3 A = mix(vec3(0), IMG_NORM_PIXEL(TEX_A, isf_FragNormCoord.xy).rgb, vec3(_TEX_A_sample));
  vec3 B = mix(vec3(0), IMG_NORM_PIXEL(TEX_B, isf_FragNormCoord.xy).rgb, vec3(_TEX_B_sample));
  vec3 V = mix(vec3(0), IMG_NORM_PIXEL(TEX_MIX, isf_FragNormCoord.xy).rgb, vec3(_TEX_MIX_sample));

  if (blend<1.0) {
    //out0 = vec4(BlendAdd(mix(A, vec3(0), abs(max(amount,0))), mix(B, vec3(0), abs(min(amount, 0)))), 1);
    out0 = vec4(BlendAdd(mix(A, vec3(0), V), mix(B, vec3(0), vec3(1)-V)), 1);
  }

  // vec3 V = mix(vec3(1.0), IMG_NORM_PIXEL(TEX_MIX, isf_FragNormCoord.xy).rgb, vec3(_TEX_MIX_sample));
  // vec3 m = vec3(_TEX_A_sample);
  // vec3 C = mix(A, B, amount * smoothstep(black, white, vec3(1)));
  // vec3 D = mix(A, B, amount * smoothstep(black, white, V));


  // vec3 color = mix(C, D, m);
  // out0 = vec4(color, 1);

}

