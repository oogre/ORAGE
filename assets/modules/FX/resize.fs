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
      "NAME" :  "x1",
      "TYPE" :  "float",
      "DEFAULT" : 0.0,
      "MIN" : 0.0,
      "MAX" : 1.0
    },
    {
      "NAME" :  "y1",
      "TYPE" :  "float",
      "DEFAULT" : 0.0,
      "MIN" : 0.0,
      "MAX" : 1.0
    },
    {
      "NAME" :  "x2",
      "TYPE" :  "float",
      "DEFAULT" : 1.0,
      "MIN" : 0.0,
      "MAX" : 1.0
    },
    {
      "NAME" :  "y2",
      "TYPE" :  "float",
      "DEFAULT" : 1.0,
      "MIN" : 0.0,
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
  ]
}*/

void main()
{
  vec3 B = IMG_NORM_PIXEL(tex1, isf_FragNormCoord.xy).rgb;

  vec2 fs = isf_FragNormCoord.xy;
  fs -= vec2(0.5);
  fs *= vec2(w, h);
  fs += vec2(0.5);
  fs = mix(vec2(x1, y1), vec2(x2, y2), fs);
  fs = mix(isf_FragNormCoord.xy, fs, vec2(max(B.r, max(B.g, B.b))));

  vec2 m = vec2(_tex1_sample);
  fs = mix(isf_FragNormCoord.xy, fs, m);

  vec3 A = IMG_NORM_PIXEL(tex0, fs).rgb;
  
  
  gl_FragColor = vec4(A, 1);

}

