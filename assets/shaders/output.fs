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
  bool isTex0 = _tex0_imgSize != vec2(1);
  bool isTex1 = _tex1_imgSize != vec2(1);
  if(isTex0 && isTex1){  
    gl_FragColor = vec4(IMG_PIXEL(tex0, isf_FragNormCoord.xy).r, IMG_PIXEL(tex1, isf_FragNormCoord.xy).g, 0, 1);
  }else if(isTex0){  
    gl_FragColor = vec4(IMG_PIXEL(tex0, isf_FragNormCoord.xy).r, 0, 0, 1);
  }else if(isTex1){  
    gl_FragColor = vec4(IMG_PIXEL(tex1, isf_FragNormCoord.xy).r, 0, 0, 1);
  }else{  
    gl_FragColor = vec4(1, 0, 0, 1);
  }
}