#version 150
float PI = 3.14159265359f;
float TWO_PI = 6.283185306f;

uniform sampler2D tex0;

layout (std140) uniform;

in vec2         vertTexCoord0;
out vec4		oColor;

vec2 _Segments[1000];

void main(){
    
    
    
    vec2 st = vertTexCoord0;
    st.x + st.y * 
    oColor = vec4(n_fragCoord.x, n_fragCoord.y, 0, 1);
}

