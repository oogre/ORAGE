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
      "NAME" :  "Y",
      "TYPE" :  "float",
      "DEFAULT" : 0.0,
      "MIN" : 0.0,
      "MAX" : 1.0
    },
    {
      "NAME" :  "Pb",
      "TYPE" :  "float",
      "DEFAULT" : 0.0,
      "MIN" : -0.5,
      "MAX" : 0.5
    },
    {
      "NAME" :  "Pr",
      "TYPE" :  "float",
      "DEFAULT" : 0.0,
      "MIN" : -0.5,
      "MAX" : 0.5
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
  vec3 A = vec3(
    1.f * Y + 0.000f * Pb + 1.574f * Pr,
    1.f * Y - 0.187f * Pb - 0.469f * Pr,
    1.f * Y + 1.856f * Pb - 0.000f * Pr
    );
  vec3 B = IMG_NORM_PIXEL(tex0, isf_FragNormCoord.xy).rgb;
  vec3 m = vec3(_tex0_sample);
  vec3 C = mix(vec3(0), A, B);
  vec3 color = mix(A, C, m);
  out0 = vec4(color, 1);
}

