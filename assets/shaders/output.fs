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
  gl_FragColor = vec4(mix(vec3(isf_FragNormCoord.x, isf_FragNormCoord.y, 0), IMG_PIXEL(tex0, isf_FragNormCoord.xy).rgb, vec3(_tex0_sample)), 1);
}

