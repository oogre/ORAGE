#version 400
	
precision highp float;	
precision highp int;	
	
#define gl_FragColor isf_FragColor
out vec4 gl_FragColor;
uniform int		PASSINDEX;
uniform vec2		RENDERSIZE;
in vec2		isf_FragNormCoord;
uniform float		TIME;
uniform float		TIMEDELTA;
uniform vec4		DATE;
uniform int		FRAMEINDEX;
uniform sampler2D		tex0;
uniform vec4		_tex0_imgRect;
uniform vec2		_tex0_imgSize;
uniform bool		_tex0_flip;

vec4 VVSAMPLER_2DBYPIXEL(sampler2D sampler, vec4 samplerImgRect, vec2 samplerImgSize, bool samplerFlip, vec2 loc)	{	
	return (samplerFlip)	
		? texture		(sampler,vec2(((loc.x/samplerImgSize.x*samplerImgRect.z)+samplerImgRect.x), (samplerImgRect.w-(loc.y/samplerImgSize.y*samplerImgRect.w)+samplerImgRect.y)))	
		: texture		(sampler,vec2(((loc.x/samplerImgSize.x*samplerImgRect.z)+samplerImgRect.x), ((loc.y/samplerImgSize.y*samplerImgRect.w)+samplerImgRect.y)));	
}	
vec4 VVSAMPLER_2DBYNORM(sampler2D sampler, vec4 samplerImgRect, vec2 samplerImgSize, bool samplerFlip, vec2 normLoc)	{	
	vec4		returnMe = VVSAMPLER_2DBYPIXEL(		sampler,samplerImgRect,samplerImgSize,samplerFlip,vec2(normLoc.x*samplerImgSize.x, normLoc.y*samplerImgSize.y));	
	return returnMe;	
}	

void main()
{
  bool isTex0 = _tex0_imgSize != vec2(1);
  if(isTex0){  
    gl_FragColor = vec4(VVSAMPLER_2DBYPIXEL(tex0, _tex0_imgRect, _tex0_imgSize, _tex0_flip, isf_FragNormCoord.xy).rgb, 1);
  }else{  
    gl_FragColor = vec4(isf_FragNormCoord.x, isf_FragNormCoord.y, 0, 1);
  }


}
