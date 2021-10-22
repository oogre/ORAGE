/*{
  "CREDIT": "by vincent evrard",
  "DESCRIPTION": "basic oscillator",
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
    }
  ]
}*/

void main()
{
  vec3 A = vec3(isf_FragNormCoord.x, isf_FragNormCoord.y, 0);
  vec3 B = IMG_NORM_PIXEL(tex0, isf_FragNormCoord.xy).rgb;
  vec3 m = vec3(_tex0_sample);
  vec3 color = mix(A, B, m);
  gl_FragColor = vec4(color, 1);
}

