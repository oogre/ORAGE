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
      "NAME" :  "R",
      "TYPE" :  "float",
      "DEFAULT" : 0.0,
      "MIN" : 0.0,
      "MAX" : 1.0
    },
    {
      "NAME" :  "G",
      "TYPE" :  "float",
      "DEFAULT" : 0.0,
      "MIN" : 0.0,
      "MAX" : 1.0
    },
    {
      "NAME" :  "B",
      "TYPE" :  "float",
      "DEFAULT" : 0.0,
      "MIN" : 0.0,
      "MAX" : 1.0
    }
  ]
}*/

void main()
{
  vec3 A = vec3(R, G, B);
  vec3 OTHER = IMG_NORM_PIXEL(tex0, isf_FragNormCoord.xy).rgb;
  vec3 m = vec3(_tex0_sample);
  vec3 C = mix(vec3(0), A, OTHER);
  vec3 color = mix(A, C, m);
  gl_FragColor = vec4(color, 1);
}

