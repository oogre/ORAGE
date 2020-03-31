#version 330 core

uniform mat4	ciModelViewProjection;
in vec4			ciPosition;
in vec2         ciTexCoord0;
out vec2        vertTexCoord0;

layout (std140) uniform;
uniform tintcorrector {
    float hue;
    float sat;
    float light;
    int modifier;
};
out vec3 weights;

void main(){
    
    float angle = hue * 3.14159265358979323846264;
	float s = sin(angle);
	float c = cos(angle);
	weights = (vec3(2.0 * c, -sqrt(3.0) * s - c, sqrt(3.0) * s - c) + 1.0) / 3.0;
    
    vertTexCoord0 = ciTexCoord0;
    gl_Position = ciModelViewProjection * ciPosition;
}