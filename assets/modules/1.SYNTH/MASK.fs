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
      "NAME" :  "MASK",
      "TYPE" :  "image"
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
  vec3 color = IMG_NORM_PIXEL(MASK, isf_FragNormCoord.xy).rgb;
  int id = int(color.r * 256) | int(color.g * 256)<<8 | int(color.b * 256) << 16;
  vec2 pos = vec2(mod(id, _MASK_imgSize.x) , id/_MASK_imgSize.x);
  out0 = IMG_PIXEL(tex0, pos);
}

