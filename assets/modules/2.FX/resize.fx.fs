/*{
  "CREDIT": "by vincent evrard",
  "DESCRIPTION": "basic trail FX",
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
      "NAME" :  "x",
      "TYPE" :  "float",
      "DEFAULT" : 0.0,
      "MIN" : -1.0,
      "MAX" : 1.0
    },
    {
      "NAME" :  "y",
      "TYPE" :  "float",
      "DEFAULT" : 0.0,
      "MIN" : -1.0,
      "MAX" : 1.0
    },
    {
      "NAME" :  "w",
      "TYPE" :  "float",
      "DEFAULT" : 1.0,
      "MIN" : -1.0,
      "MAX" : 1.0
    },
    {
      "NAME" :  "h",
      "TYPE" :  "float",
      "DEFAULT" : 1.0,
      "MIN" : -1.0,
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
   vec2 fs = isf_FragNormCoord.xy;

  fs -= vec2(0.5);
  
  fs /= vec2(w, h);
  fs -= vec2(x * (1.0-abs(w)), y * (1.0-abs(h)));
  fs += vec2(0.5);

  vec2 areaTL = step(vec2(0), fs);
  vec2 areaBR = step(vec2(1), fs);
  vec3 select = vec3(areaTL.x==1.0 && areaTL.y==1.0 && areaBR.x==0.0 && areaBR.y==0.0);



  vec3 A = mix(IMG_NORM_PIXEL(tex1, isf_FragNormCoord.xy).rgb, IMG_NORM_PIXEL(tex0, fs).rgb, select);

  
  // fs = mix(isf_FragNormCoord.xy, fs, vec2(max(B.r, max(B.g, B.b))));

  // vec2 m = vec2(_tex1_sample);
  // fs = mix(isf_FragNormCoord.xy, fs, m);

  
  
  out0 = vec4(A, 1);

}

