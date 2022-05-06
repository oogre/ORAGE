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

void main()
{
  int x = int(isf_FragNormCoord.x * WIDTH);
  int y = int(isf_FragNormCoord.y * HEIGHT);

  if(blend < 1){
    
  }
  else if(blend < 2){
    x = int(mix(int(WIDTH)-x, x, mod(y, 2)));  
  }
  else if(blend < 3){
    y = int(mix(int(HEIGHT)-y, y, mod(x, 2)));
  }
  else{
    x = int(mix(int(WIDTH)-x, x, mod(y, 2)));  
    y = int(mix(int(HEIGHT)-y, y, mod(x, 2)));  
  }
  

  int id =  int(x + y * WIDTH);
  vec3 color = idToColor(id);
  out0 = vec4(color, 1);
}

