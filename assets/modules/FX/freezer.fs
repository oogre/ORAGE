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
      "NAME" :  "tex1",
      "TYPE" :  "image"
    },
    {
      "NAME" :  "_tex1_sample",
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
  ]
}*/

void main()
{
  vec3 A = IMG_NORM_PIXEL(tex0, isf_FragNormCoord.xy).rgb;
  vec3 B = IMG_NORM_PIXEL(tex1, isf_FragNormCoord.xy).rgb;
  vec3 V = IMG_NORM_PIXEL(tex2, isf_FragNormCoord.xy).rgb;
  vec3 m = vec3(_tex0_sample);
  vec3 C = mix(A, B, amount * smoothstep(black, white, vec3(1)));
  vec3 D = mix(A, B, amount * smoothstep(black, white, V));
  vec3 color = mix(C, D, m);
  gl_FragColor = vec4(color, 1);

}

