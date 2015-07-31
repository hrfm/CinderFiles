#version 120

// Textures
uniform sampler2D tex;
uniform float time;
uniform vec2 resolution;

uniform float bpm_position;

vec4 trans(vec2 p)
{
    
    const float segment_x = 8.0;
    const float segment_y = 0.3;
    
    const float height = 0.5;
    float r = height/p.y * segment_y;
    
    vec2 pos = vec2(p.x*r, r);
    
    return texture2D( tex, mod( pos, 1.0 ) ) * ( 1.0 - ( 0.1 - abs( 0.5 - (gl_FragCoord.y/resolution.y) ) ) / 0.1 );
    
}

void main() {
    vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / resolution.y;
    gl_FragColor = trans(pos);
}