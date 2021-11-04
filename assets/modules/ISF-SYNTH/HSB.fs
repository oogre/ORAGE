/*{
  "CREDIT": "by vincent evrard",
  "DESCRIPTION": "basic HSB color generator",
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
      "NAME" :  "H",
      "TYPE" :  "float",
      "DEFAULT" : 0.0,
      "MIN" : 0.0,
      "MAX" : 360.0
    },
    {
      "NAME" :  "S",
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
  float C = B * S;
  float X = C * (1.0-abs(mod((H / 60.0), 2.0) - 1.0));
  float m = B - C;
  vec3 tmp = vec3(0.0);
  if(H < 60.0){
    tmp = vec3(C, X, 0);
  }else if(H < 120.0){
    tmp = vec3(X, C, 0);
  }else if(H < 120.0){
    tmp = vec3(X, C, 0);
  }else if(H < 180.0){
    tmp = vec3(0, C, X);
  }else if(H < 240.0){
    tmp = vec3(0, X, C);
  }else if(H < 300.0){
    tmp = vec3(X, 0, C);
  }else{
    tmp = vec3(C, 0, X);
  }
  vec3 A = vec3( tmp.r + m, tmp.g + m, tmp.b + m);
  vec3 B = IMG_NORM_PIXEL(tex0, isf_FragNormCoord.xy).rgb;
  vec3 _m = vec3(_tex0_sample);
  vec3 COL = mix(vec3(0), A, B);
  vec3 color = mix(A, COL, _m);
  gl_FragColor = vec4(color, 1);
    //  gl_FragColor = vec4(0, 1, 1, 1);
}
