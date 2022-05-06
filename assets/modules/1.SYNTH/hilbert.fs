/*{
  "CREDIT": "by vincent evrard",
  "DESCRIPTION": "basic YPbPr color generator",
  "ISFVSN": "2",
  "CATEGORIES": [
    "orage"
  ],
  "INPUTS": [
    {
      "NAME" :  "blend",
      "TYPE" :  "float",
      "DEFAULT" : 0.0,
      "MIN" : 0.0,
      "MAX" : 12.0
    }
  ],
  "OUTPUTS": [
    {
      "NAME" :  "out0",
      "TYPE" :  "image"
    }
  ]
}*/

vec3 idToColor (int id){
  return vec3 ( id & 0xff, id >> 8 & 0xff, id >> 16 & 0xff ) / vec3(256.0); 
}


vec2 HilbertCurve(int i, int order) {// order = 7
  int index = i & 3;
  vec2 v;
  if(i==0){
    v = vec2(0, 0);
  } else if(i==1){
    v = vec2(0, 1);
  } else if(i==2){
    v = vec2(1, 1);
  } else if(i==3){
    v = vec2(1, 0);
  }
  
  for (int j = 1; j < order; j ++) {
    i = i >> 2;
    index = i & 3;
    float len = pow(2, j);
    if (index == 0) {
      float t = v.x;
      v.x = v.y;
      v.y = t;
    } else if (index == 1) {
      v.y+= len;
    } else if (index == 2) {
      v.x+= len;
      v.y+= len;
    } else if (index == 3) {
      float t = len-1-v.x;
      v.x = len-1-v.y;
      v.y = t;
      v.x+= len;
    }
  }
  return v;
}


void main()
{
  int x = int(isf_FragNormCoord.x * WIDTH);
  int y = int(isf_FragNormCoord.y * HEIGHT);

  // if(blend < 1){
    
  // }
  // else if(blend < 2){
  //   x = int(mix(int(WIDTH)-x, x, mod(y, 2)));  
  // }
  // else if(blend < 3){
  //   y = int(mix(int(HEIGHT)-y, y, mod(x, 2)));
  // }
  // else{
  //   x = int(mix(int(WIDTH)-x, x, mod(y, 2)));  
  //   y = int(mix(int(HEIGHT)-y, y, mod(x, 2)));  
  // }

  int id =  int(x + y * WIDTH);
  int order = 3;
  int N = int(pow(2, order));
  int  total = N * N;
  int closestPoint = 0;
  int lastDist = 10000000;
  for(int i = 0 ; i < total ; i ++){
    vec2 pos = HilbertCurve(int(mod(i, total)), order);  
    vec2 m = (isf_FragNormCoord.xy - pos);
    float sqdist = m.x * m.x + m.y * m.y;
    lastDist = int(min(lastDist, sqdist));
  }
  out0 = vec4(vec3(lastDist), 1);
}
