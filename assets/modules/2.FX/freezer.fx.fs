/*{
  "CREDIT": "by vincent evrard",
  "DESCRIPTION": "basic YPbPr color generator",
  "ISFVSN": "2",
  "CATEGORIES": [
    "orage"
  ],
  "INPUTS": [
  	{
      "NAME" :  "tex0",
      "TYPE" :  "image"
    },
    {
      "NAME" :  "_tex0_sample",
      "TYPE" :  "long",
      "DEFAULT": 0
    },
    {
      "NAME" :  "OLD",
      "TYPE" :  "image"
    },
    {
      "NAME" :  "_OLD_sample",
      "TYPE" :  "long",
      "DEFAULT": 0
    },
    {
      "NAME" :  "tex2",
      "TYPE" :  "image"
    },
    {
      "NAME" :  "_tex2_sample",
      "TYPE" :  "long",
      "DEFAULT": 0
    },
    {
      "NAME" :  "amount",
      "TYPE" :  "float",
      "DEFAULT" : 1.0,
      "MIN" : 0.0,
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

void main()
{
  vec3 A = IMG_NORM_PIXEL(tex0, isf_FragNormCoord.xy).rgb;
  vec3 B = IMG_NORM_PIXEL(OLD, isf_FragNormCoord.xy).rgb;
  vec3 V = mix(vec3(1.0), IMG_NORM_PIXEL(tex2, isf_FragNormCoord.xy).rgb, _tex2_sample);
  vec3 m = vec3(_tex0_sample);
  vec3 C = mix(A, B, amount * smoothstep(black, white, vec3(1)));
  vec3 D = mix(A, B, amount * smoothstep(black, white, V));


  vec3 color = mix(C, D, m);
  out0 = vec4(color, 1);

}

