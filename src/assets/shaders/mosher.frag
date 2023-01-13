
#version 150
float PI = 3.14159265359f;
float TWO_PI = 6.283185306f;

uniform sampler2D tex0;
uniform sampler2D tex1;

layout (std140) uniform;
uniform mosher {
    float gainX; //gain3
    float gainY; //gain3
    float x   ;
    float y   ;
};


in vec2         vertTexCoord0;
out vec4		oColor;


vec2 rotate2D(vec2 _st, float _angle){
    _st =  mat2(cos(_angle), -sin(_angle),
                sin(_angle),  cos(_angle) ) * _st;
    return _st;
}

void main()
{
    vec2 pos = vec2(x, y);
    vec2 n_fragCoord = vertTexCoord0;
    
    vec2 m_fragCoord = n_fragCoord;
    m_fragCoord = m_fragCoord * 2.0 - 1.0;
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
    
    n_fragCoord.x *= (1 - (gainX * modifierV)); //pow(modifierV, modifierV);
    n_fragCoord.y *= (1 - (gainY * modifierV)); //pow(modifierV, modifierV);
    n_fragCoord -= pos;
    n_fragCoord = (n_fragCoord + 1.0) * 0.5;
    
        if(n_fragCoord.x < 0 || n_fragCoord.x > 1 || n_fragCoord.y < 0 || n_fragCoord.y > 1){
            oColor = vec4(vec3(0), 1);
        }else{
            oColor = vec4(texture(tex0, n_fragCoord).rgb, 1);
        }
    
    
}
