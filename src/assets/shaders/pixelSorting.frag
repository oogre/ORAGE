#version 400 core

uniform sampler2D tex0; // NEW IMAGE

layout (std140) uniform;
uniform pixelSorting {
    uniform float start;
    uniform float stop;
    uniform int width;
    uniform int height;

};
in vec2         vertTexCoord0;
out vec4		oColor;


void sort(in vec3 _input [100], out vec3 _output [100], in int len){
    _output = _input;
    bool sorted = false;
    while(!sorted){
        sorted = true;
        for(int i = 0 ; i < len-1 ; i ++){
            if(any(greaterThan(_output[i], _output[i+1]))){
                vec3 tmp = _output[i];
                _output[i] = _output[i+1];
                _output[i+1] = tmp;
                sorted = false;
            }
        }
    }
}


void main()
{
    vec2 n_fragCoord = vertTexCoord0;
    vec3 col [100];
    float posInc = 1.0 / height;
    float pixelPointer = 0.0;
    for(int i = 0 ; i < 100 ; i ++, pixelPointer += posInc){
        col[i] = texture(tex0, vec2(vertTexCoord0.x, pixelPointer)).rgb;
    }
    vec3 _output [100];
    sort(col, _output, 100);
    oColor = vec4(_output[vertTexCoord0.y * 100], 1);
    
}