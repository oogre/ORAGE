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
      "NAME" :  "tex1",
      "TYPE" :  "image"
    },
    {
      "NAME" :  "_tex1_sample",
      "TYPE" :  "long",
      "DEFAULT": 0
    },
    {
      "NAME" :  "sizeX",
      "TYPE" :  "float",
      "DEFAULT" : 0.0,
      "MIN" : -1.0,
      "MAX" : 1.0
    },
    {
      "NAME" :  "sizeY",
      "TYPE" :  "float",
      "DEFAULT" : 0.0,
      "MIN" : -1.0,
      "MAX" : 1.0
    },
    {
      "NAME" :  "mirror",
      "TYPE" :  "float",
      "DEFAULT" : 1.0,
      "MIN" : 0.0,
      "MAX" : 1.0
    },
    {
      "NAME" :  "centerX",
      "TYPE" :  "float",
      "DEFAULT" : 0.5,
      "MIN" : 0.0,
      "MAX" : 1.0
    },
    {
      "NAME" :  "centerY",
      "TYPE" :  "float",
      "DEFAULT" : 0.5,
      "MIN" : 0.0,
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


float PI = 3.14159265359;
float TWO_PI = 6.283185306;

vec2 rotate2D(vec2 _st, float _angle, vec2 _rotC){
    _st -= _rotC;
    _st =  mat2(cos(_angle), -sin(_angle),
                sin(_angle),  cos(_angle) ) * _st;
    _st += _rotC;
    return _st;
}

vec2 mirrorTilePattern(vec2 _st, vec2 _size, vec2 _center){
    
    //  Scale the coordinate system by tile
    _st -= _center;
    _st *= _size;
    _st += _center;
    
    //  Give each cell an index number
    //  according to its position
    float index = 0.0;
    index += step(1., mod(_st.x,2.0));
    index += step(1., mod(_st.y,2.0))*2.0;
    
    //      |
    //  0   |   1
    //      |
    //--------------
    //      |
    //  2   |   3
    //      |
    
    _st = fract(_st);
    if(index == 1.0 || index == 3.0)
    {
        _st.x = mix(_st.x, 1.0 - _st.x, mirror);
    }
    if(index == 2.0 || index == 3.0)
    {
        _st.y = mix(_st.y, 1.0 - _st.y, mirror);
    }
    return _st;
}

void main()
{
  vec2 fs = isf_FragNormCoord.xy;
  vec2 center = vec2(centerX, centerY);
  vec2 size = vec2(sizeX, sizeY) + vec2(1);

  if(size.x>1)size.x =mix(1.0, 10.0, size.x-1);
  if(size.y>1)size.y =mix(1.0, 10.0, size.y-1);
  vec3 B = IMG_NORM_PIXEL(tex1, fs).rgb;

  fs = mirrorTilePattern(fs, size, center);
  if(_tex1_sample){
   fs = mix(isf_FragNormCoord.xy, fs, vec2(max(B.r, max(B.g, B.b))));
  }
  vec3 color = IMG_NORM_PIXEL(tex0, fs).rgb;
  out0 = vec4(color, 1);

}

