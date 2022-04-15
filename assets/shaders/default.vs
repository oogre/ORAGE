#version 400
	
precision highp float;	
precision highp int;	

uniform mat4	ciModelViewProjection;
in vec4			ciPosition;	
in vec2			ciTexCoord0;
out vec2		isf_FragNormCoord;

uniform int		PASSINDEX;
uniform vec2	RENDERSIZE;
uniform float	TIME;
uniform float	TIMEDELTA;
uniform vec4	DATE;
uniform int		FRAMEINDEX;
	
void main(void)	{	
	// isf_FragNormCoord = ((ciModelViewProjection * ciPosition).xy + vec2(1)) * 0.5;
	// gl_Position = ciModelViewProjection * ciPosition;
	isf_FragNormCoord = ciTexCoord0 ;
    gl_Position = ciModelViewProjection * ciPosition;
}