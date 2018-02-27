
#version 330 core

uniform sampler2D tex0;
uniform sampler2D tex1;
layout (std140) uniform;

uniform blur {
    float amountX;
    float amountY;
    bool blurActive;
    float width;
    float height;
};

in vec2         vertTexCoord0;
out vec4		oColor;

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


void main()
{
    vec4 color = vec4(0.0);
    vec2 blur  = vec2(amountX, amountY);
    if(blurActive){
        vec3 modifierValue = texture(tex1, vertTexCoord0).rgb;
        float modifierV = max(modifierValue.x, max(modifierValue.y, modifierValue.z)) * 0.20;
        blur = blur * modifierV;
    }
    vec2 furthest = blur * 4.0;
    vec2 further  = blur * 3.0;
    vec2 closer   = blur * 2.0;
    vec2 closest  = blur;
    
    vec2 d = vec2(width, height);
    vec2 st = vec2 (0);
    
    st = audela(d * (gl_FragCoord.xy + vec2(-furthest.x, 0.0)));
    color += 1.0 * texture(tex0, st);
    st = audela(d * (gl_FragCoord.xy + vec2(furthest.x, 0.0)));
    color += 1.0 * texture(tex0, st);
    st = audela(d * (gl_FragCoord.xy + vec2(0.0, furthest.y)));
    color += 1.0 * texture(tex0, st);
    st = audela(d * (gl_FragCoord.xy + vec2(0.0, -furthest.y)));
    color += 1.0 * texture(tex0, st);

    st = audela(d * (gl_FragCoord.xy + vec2(-further.x,  0.0)));
    color += 2.0 * texture(tex0, st);
    st = audela(d * (gl_FragCoord.xy + vec2(further.x,  0.0)));
    color += 2.0 * texture(tex0, st);
    st = audela(d * (gl_FragCoord.xy + vec2(0.0, further.y)));
    color += 2.0 * texture(tex0, st);
    st = audela(d * (gl_FragCoord.xy + vec2(0.0, -further.y)));
    color += 2.0 * texture(tex0, st);

    st = audela(d * (gl_FragCoord.xy + vec2(-closer.x,   0.0)));
    color += 3.0 * texture(tex0, st);
    st = audela(d * (gl_FragCoord.xy + vec2(closer.x,   0.0)));
    color += 3.0 * texture(tex0, st);
    st = audela(d * (gl_FragCoord.xy + vec2(0.0, closer.y)));
    color += 3.0 * texture(tex0, st);
    st = audela(d * (gl_FragCoord.xy + vec2(0.0, -closer.y)));
    color += 3.0 * texture(tex0, st);

    st = audela(d * (gl_FragCoord.xy + vec2(-closest.x,  0.0)));
    color += 4.0 * texture(tex0, st);
    st = audela(d * (gl_FragCoord.xy + vec2(closest.x,  0.0)));
    color += 4.0 * texture(tex0, st);
    st = audela(d * (gl_FragCoord.xy + vec2(0.0, closest.y)));
    color += 4.0 * texture(tex0, st);
    st = audela(d * (gl_FragCoord.xy + vec2(0.0, -closest.y)));
    color += 4.0 * texture(tex0, st);

    st = audela(d * (gl_FragCoord.xy + vec2(amountX, amountY)));
    color += 10.0 * texture(tex0, st);
    
    color *= 0.02;
    
    oColor = color;
}




