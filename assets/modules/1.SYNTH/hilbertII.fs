
/*{
  "CREDIT": "by vincent evrard",
  "DESCRIPTION": "basic YPbPr color generator",
  "ISFVSN": "2",
  "CATEGORIES": [
    "orage"
  ],
  "INPUTS": [
    
  ],
  "OUTPUTS": [
    {
      "NAME" :  "out0",
      "TYPE" :  "image"
    }
  ]
}*/
int i = 0;


mat2 rotation(float a) {
    // input is 0., 1., 2., 3.


    // for a quadrant [0, 1, 2, 3] from UL in clockwise order,
    // return the rotation for all curves beneath it.
    if (a < 0.5) {
        // rot(90)
       return mat2( 0.,  1. ,
                    1.,  0.);
    }
    if (a <= 2.5) {
       // Two of the same shape.
       return mat2( 1.,  0.,
                    0.,  1.);
    }
    //rot(-90)
       return mat2(  0.,  -1.,
                     -1.,  0.);
}

vec2 scale(vec2 fs, float depth){
  fs *= 2;
  int x = int(fs.x);
  int y = int(fs.y);
  
  if(x%2 == 0 && y%2 == 0){
    i += 0;
  }
  if(x%2 == 0 && y%2 == 1){
    i += 1;
  }
  if(x%2 == 1 && y%2 == 1){
    i += 2;
  }
  if(x%2 == 1 && y%2 == 0){
    i += 3;
  }
  return fs;
}

void hilbert(vec2 pos, float depth){

  if(pos.x < 0.5 && pos.y < 0.5){
    return 0;
  }
  if(pos.x < 0.5 && pos.y < 0.5){
    return 0;
  }


}


void main()
{
  vec2 fs = isf_FragNormCoord;
  for(int i = 0 ; i < 14 ; i ++){
    fs = scale(fs);
  }
  

  
  
  out0 = vec4(vec3(i/(14*3.0)), 1);
}
