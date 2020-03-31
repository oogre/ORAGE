#version 150

uniform sampler2D tex0; // NEW IMAGE
uniform sampler2D tex2; // OLD IMAGE
uniform sampler2D tex1; // SELECTOR

layout (std140) uniform;
uniform freezer {
 float amount;
    uniform float black;
    uniform float white;
    uniform bool selectorActive;
};


in vec2         vertTexCoord0;
out vec4		oColor;

void main()
{
    vec2 n_fragCoord = vertTexCoord0;

    if(selectorActive){
        oColor = vec4(mix(texture(tex0, n_fragCoord).rgb, texture(tex2, n_fragCoord).rgb, amount * smoothstep(black, white, texture(tex1, n_fragCoord).rgb)), 1);
    }else{
        oColor = vec4(mix(texture(tex0, n_fragCoord).rgb, texture(tex2, n_fragCoord).rgb, amount * smoothstep(black, white, vec3(1))), 1);
    
    }
    
    
}