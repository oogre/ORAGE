#version 150
float PI = 3.14159265359f;
float TWO_PI = 6.283185306f;

uniform sampler2D tex0;
uniform sampler2D tex1;
layout (std140) uniform;

uniform tile {
    float tileSizeX;
    float tileSizeY;
    float tileRotation;
    float tileCenterX;
    float tileCenterY;
    float preRotation;
    float postRotation;
    float rotCenterX;
    float rotCenterY;
    int modifier;
};

in vec2         vertTexCoord0;
out vec4		oColor;

vec2 rotate2D(vec2 _st, float _angle){
    _st -= vec2(rotCenterX, rotCenterY);
    _st =  mat2(cos(_angle), -sin(_angle),
                sin(_angle),  cos(_angle) ) * _st;
    _st += vec2(rotCenterX, rotCenterY);
    return _st;
}

vec2 mirrorTilePattern(vec2 _st){
    //  Scale the coordinate system by tile
    _st -= vec2(tileCenterX, tileCenterY);
    _st *= (vec2(tileSizeX, tileSizeY));
    _st += vec2(tileCenterX, tileCenterY);
    
    //  Give each cell an index number
    //  according to its position
    float index = 0.0;
    index += step(1., mod(_st.x,2.0));
    index += step(1., mod(_st.y,2.0))*2.0;
    
    //      |
    //  0   |   1
    //      |
    //--------------
    //      |
    //  2   |   3
    //      |
    
    _st = fract(_st);
    
    
    if(index == 1.0 || index == 3.0)
    {
        _st.x = 1.0 - _st.x;
    }
    if(index == 3.0 || index == 2.0)
    {
        _st.y = 1.0 - _st.y;
    }
    
    if(index == 1.0)
    {
        _st = rotate2D(_st, tileRotation * PI/2);
    } else if(index == 2.0)
    {
        _st = rotate2D(_st, tileRotation * -PI/2.0);
    } else if(index == 3.0)
    {
        _st = rotate2D(_st, tileRotation * PI);
    }
    return _st;
}

void main()
{
    vec2 n_fragCoord = vertTexCoord0;
    vec3 strenght = texture(tex1, n_fragCoord).rgb;
    float s = max(max(strenght.r, strenght.g), strenght.b);
    if(modifier!=1){
        s = 1;
    }
    n_fragCoord = s * rotate2D(n_fragCoord, postRotation * TWO_PI) + (1.0-s) * vertTexCoord0;
    n_fragCoord = s * mirrorTilePattern(n_fragCoord) + (1.0-s) * vertTexCoord0 ;
    n_fragCoord = s * rotate2D(n_fragCoord, preRotation * TWO_PI) + (1.0-s) * vertTexCoord0;
    oColor = texture(tex0, n_fragCoord);
}

