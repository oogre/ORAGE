#version 150

uniform sampler2D tex0;
uniform sampler2D tex1;

layout (std140) uniform;
uniform mosher {
    float map1; //gain
    float map2; //gain2
    float gain; //gain3
    float x   ;
    float y   ;
};


in vec2         vertTexCoord0;
out vec4		oColor;

void main()
{
    vec2 pos = vec2(x, y);
    vec2 n_fragCoord = vertTexCoord0;
    
    vec2 m_fragCoord = n_fragCoord;
    m_fragCoord = m_fragCoord * 2.0 - 1.0;
    m_fragCoord *= map2;
    m_fragCoord = (m_fragCoord + 1.0) * 0.5;
    
    if(m_fragCoord.x < 0 || m_fragCoord.x > 1){
        m_fragCoord.x = 1 - m_fragCoord.x;
    }
    if(m_fragCoord.y < 0 || m_fragCoord.y > 1){
        m_fragCoord.y = 1 - m_fragCoord.y;
    }
    
    m_fragCoord = fract(m_fragCoord);
    
    vec3 modifierValue = texture(tex1, m_fragCoord).rgb;
    
    float modifierV = max(modifierValue.x, max(modifierValue.y, modifierValue.z));
    
    n_fragCoord = n_fragCoord * 2.0 - 1.0;
    n_fragCoord += pos;
    n_fragCoord *= (map1 - (gain * modifierV)); //pow(modifierV, modifierV);
    n_fragCoord -= pos;
    n_fragCoord = (n_fragCoord + 1.0) * 0.5;
    
    
    if(n_fragCoord.x < 0 || n_fragCoord.x > 1){
        n_fragCoord.x = 1 - n_fragCoord.x;
    }
    if(n_fragCoord.y < 0 || n_fragCoord.y > 1){
        n_fragCoord.y = 1 - n_fragCoord.y;
    }
    
    n_fragCoord = fract(n_fragCoord);
    
    oColor = vec4(texture(tex0, n_fragCoord).rgb, 1);
}