#version 120

// Textures
uniform sampler2D tex;
uniform float time;
uniform vec2 resolution;

uniform int   type;
uniform float bpm_position;

vec4 trans(vec2 p)
{
    
    const float segment_x = 8.0;
    const float segment_y = 4.0;
    
    float theta = 0.25 + ( atan(p.y, p.x) / 6.283185307 ) * segment_x;
    float r = length(p) * segment_y;
    
    //*
    
    if( type == 1 ){
        theta += r * bpm_position * 0.3;
    }
    
    const float radius = 1.0;
    vec2 pos = vec2(theta, radius/r);
    
    if( type == 2 ){
        if( 1.0 <= mod(pos.y,2.0) ){
            pos.x += 0.5 + 1.0 * bpm_position;
        }else{
            pos.x -= 1.0 * bpm_position;
        }
    }else if( type == 3 ){
        if( 1.0 <= mod(pos.y,2.0) ){
            pos.x += 0.5 + 1.0 * bpm_position;
        }
    }
    
    //*/

    return texture2D( tex, mod( pos, 1.0 ) ) * distance(vec2(0.5),gl_FragCoord.xy/resolution) / 0.09;
    
}

void main() {
    vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / resolution.y;
    gl_FragColor = trans(pos);
}