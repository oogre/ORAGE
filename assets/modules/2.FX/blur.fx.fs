/*{
  "CREDIT": "by vincent evrard",
  "DESCRIPTION": "basic YPbPr color generator",
  "ISFVSN": "2",
  "CATEGORIES": [
    "orage"
  ],
  "INPUTS": [
  	{
      "NAME" :  "TEX_A",
      "TYPE" :  "image",
      "MAIN" : true
    },
    {
      "NAME" :  "TEX_B",
      "TYPE" :  "image"
    },
    {
      "NAME" :  "TEX_C",
      "TYPE" :  "image"
    },
    {
      "NAME" :  "TEX_D",
      "TYPE" :  "image"
    },
    {
      "NAME" :  "amountX",
      "TYPE" :  "float",
      "DEFAULT" : 0.0,
      "MIN" : 0.0,
      "MAX" : 100.0
    },
    {
      "NAME" :  "amountY",
      "TYPE" :  "float",
      "DEFAULT" : 0.0,
      "MIN" : 0.0,
      "MAX" : 100.0
    }

  ],
  "OUTPUTS": [
    {
      "NAME" :  "out0",
      "TYPE" :  "image"
    },
    {
      "NAME" :  "out1",
      "TYPE" :  "image"
    },
    {
      "NAME" :  "out2",
      "TYPE" :  "image"
    }
  ]
}*/

#include "shaders/PhotoshopMathFP.glsl"

vec2 audela(vec2 n_fragCoord){
    if(n_fragCoord.x < 0 || n_fragCoord.x > 1){
        n_fragCoord.x = 1 - n_fragCoord.x;
    }
    if(n_fragCoord.y < 0 || n_fragCoord.y > 1){
        n_fragCoord.y = 1 - n_fragCoord.y;
    }
    n_fragCoord = fract(n_fragCoord);
    return n_fragCoord;
}

vec4 blur13PassA(vec2 uv, vec2 resolution, vec2 direction) {
  vec4 color = vec4(0.0);
  vec2 off1 = vec2(1.411764705882353) * direction;
  vec2 off2 = vec2(3.2941176470588234) * direction;
  vec2 off3 = vec2(5.176470588235294) * direction;
  color += IMG_NORM_PIXEL(TEX_A, uv) * 0.1964825501511404;
  color += IMG_NORM_PIXEL(TEX_A, uv + (off1 / resolution)) * 0.2969069646728344;
  color += IMG_NORM_PIXEL(TEX_A, uv - (off1 / resolution)) * 0.2969069646728344;
  color += IMG_NORM_PIXEL(TEX_A, uv + (off2 / resolution)) * 0.09447039785044732;
  color += IMG_NORM_PIXEL(TEX_A, uv - (off2 / resolution)) * 0.09447039785044732;
  color += IMG_NORM_PIXEL(TEX_A, uv + (off3 / resolution)) * 0.010381362401148057;
  color += IMG_NORM_PIXEL(TEX_A, uv - (off3 / resolution)) * 0.010381362401148057;
  return color;
}


vec4 blur13PassB(vec2 uv, vec2 resolution, vec2 direction) {
  vec4 color = vec4(0.0);
  vec2 off1 = vec2(1.411764705882353) * direction;
  vec2 off2 = vec2(3.2941176470588234) * direction;
  vec2 off3 = vec2(5.176470588235294) * direction;
  color += IMG_NORM_PIXEL(TEX_B, uv) * 0.1964825501511404;
  color += IMG_NORM_PIXEL(TEX_B, uv + (off1 / resolution)) * 0.2969069646728344;
  color += IMG_NORM_PIXEL(TEX_B, uv - (off1 / resolution)) * 0.2969069646728344;
  color += IMG_NORM_PIXEL(TEX_B, uv + (off2 / resolution)) * 0.09447039785044732;
  color += IMG_NORM_PIXEL(TEX_B, uv - (off2 / resolution)) * 0.09447039785044732;
  color += IMG_NORM_PIXEL(TEX_B, uv + (off3 / resolution)) * 0.010381362401148057;
  color += IMG_NORM_PIXEL(TEX_B, uv - (off3 / resolution)) * 0.010381362401148057;
  return color;
}


vec4 blur13PassC(vec2 uv, vec2 resolution, vec2 direction) {
  vec4 color = vec4(0.0);
  vec2 off1 = vec2(1.411764705882353) * direction;
  vec2 off2 = vec2(3.2941176470588234) * direction;
  vec2 off3 = vec2(5.176470588235294) * direction;
  color += IMG_NORM_PIXEL(TEX_C, uv) * 0.1964825501511404;
  color += IMG_NORM_PIXEL(TEX_C, uv + (off1 / resolution)) * 0.2969069646728344;
  color += IMG_NORM_PIXEL(TEX_C, uv - (off1 / resolution)) * 0.2969069646728344;
  color += IMG_NORM_PIXEL(TEX_C, uv + (off2 / resolution)) * 0.09447039785044732;
  color += IMG_NORM_PIXEL(TEX_C, uv - (off2 / resolution)) * 0.09447039785044732;
  color += IMG_NORM_PIXEL(TEX_C, uv + (off3 / resolution)) * 0.010381362401148057;
  color += IMG_NORM_PIXEL(TEX_C, uv - (off3 / resolution)) * 0.010381362401148057;
  return color;
}


void main()
{
  // vec2 blur  = ;
 if(_TEX_D_sample){
      vec3 modifierValue = IMG_NORM_PIXEL(TEX_D, isf_FragNormCoord.xy).rgb;
      float modifierV = max(modifierValue.x, max(modifierValue.y, modifierValue.z)) * 0.20;
      // blur = blur * modifierV;
  }


  out0 = vec4(BlendOverlay(
    blur13PassA(isf_FragNormCoord.xy, vec2(WIDTH, HEIGHT), vec2(amountX, 0)*0.33),
    blur13PassA(isf_FragNormCoord.xy, vec2(WIDTH, HEIGHT), vec2(0, amountY)*0.33)
  ), 1);
  out1 = vec4(BlendOverlay(
    blur13PassB(isf_FragNormCoord.xy, vec2(WIDTH, HEIGHT), vec2(amountX, 0)*0.5),
    blur13PassB(isf_FragNormCoord.xy, vec2(WIDTH, HEIGHT), vec2(0, amountY)*0.5)
  ), 1);
  out2 = vec4(BlendOverlay(
    blur13PassC(isf_FragNormCoord.xy, vec2(WIDTH, HEIGHT), vec2(amountX, 0)),
    blur13PassC(isf_FragNormCoord.xy, vec2(WIDTH, HEIGHT), vec2(0, amountY))
  ), 1);

}