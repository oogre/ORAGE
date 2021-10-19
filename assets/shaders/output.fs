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
    }
  ]
}*/

void main()
{
  bool isTex0 = _tex0_imgSize != vec2(1);
  if(isTex0){  
    gl_FragColor = vec4(IMG_PIXEL(tex0, isf_FragNormCoord.xy).rgb, 1);
  }else{  
    gl_FragColor = vec4(isf_FragNormCoord.x, isf_FragNormCoord.y, 0, 1);
  }


}

