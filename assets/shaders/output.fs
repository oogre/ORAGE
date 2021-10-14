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
    },{
      "NAME" :  "tex1",
      "TYPE" :  "image"
    }
  ]
}*/

void main()
{
    gl_FragColor = IMG_NORM_PIXEL(tex0, isf_FragNormCoord.xy);
}