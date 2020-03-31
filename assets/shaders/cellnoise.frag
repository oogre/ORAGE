#version 330 core

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D tex0;

in vec2		vTexCoord0;
uniform vec2 u_resolution;
out vec4        oColor;

layout (std140) uniform;
uniform cellNoise {
    float movex;
    float movey;
    float size;
    float square;
    float power;
};


vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

void main() {
    vec2 st = vTexCoord0;
    
    st = pow (st, vec2(distance(st, vec2(0.5))));
    st *= size;
    
    vec3 color = vec3(.0);
    
    // Tile the space
    vec2 i_st = floor(st);
    vec2 f_st = fract(st);
    
    float m_dist = 0.9;  // minimun distance
    
    for (int y= -1; y <= 1; y++) {
        for (int x= -1; x <= 1; x++) {
            // Neighbor place in the grid
            vec2 neighbor = vec2(float(x),float(y));
            
            // Random position from current + neighbor place in the grid
            vec2 point = random2(i_st + neighbor);
            
            // Animate the point
            //point = 0.5 + 0.5*sin(6.2831*point);
            //point = .5 + 0.5*sin(6.2831*u_mouse.x/u_resolution.x + 6.2831*point);
            point = sin(vec2(movex, movey)*6.2831 + square*6.2831*point);
            // Vector between the pixel and the point
            vec2 diff = neighbor + point - f_st;
            
            // Distance to the point
            float dist = length(diff);
            
            // Keep the closer distance
            m_dist = min(m_dist, dist);
        }
    }
    
    // Draw the min distance (distance field)
    color += texture(tex0, vTexCoord0*vec2(m_dist)).rgb;
    
    
    // Draw cell center
    //color += 1.-step(.02, m_dist);
    
    // Draw grid
    //color.r += step(.98, f_st.x) + step(.98, f_st.y);
    
    // Show isolines
    //color -= step(.7,abs(sin(27.0*m_dist)))*.5;
    
    oColor = vec4(color,1.0);
}