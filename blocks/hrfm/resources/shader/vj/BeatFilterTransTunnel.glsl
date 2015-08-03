#version 120

// Textures
uniform sampler2D tex;
uniform float time;
uniform vec2 resolution;

uniform int   type;
uniform float bpm_position;

uniform float segment_x;
uniform float segment_y;
uniform float move;

vec4 trans(vec2 p)
{
    
    float theta = 0.25 + ( atan(p.y, p.x) / 6.283185307 ) * segment_x;
    float r = length(p) * segment_y;
    
    if( type == 1 ){
        theta += r * bpm_position * 0.1 * move;
    }
    /*
    float move = 1.0 * bpm_position * (r-mod(r,1.0)) * 0.1;
    if( type == 2 ){
        if( 1.0 <= mod(r,2.0) ){
            theta += 0.5 + move;
        }else{
            theta -= move;
        }
    }else if( type == 3 ){
        if( 1.0 <= mod(r,2.0) ){
            theta += 0.5 + move;
        }
    }
    //*/
    
    const float radius = 1.0;
    vec2 pos = vec2(theta, radius/r);
    
    //*
    float m = 1.0 * bpm_position * (pos.y-mod(pos.y,1.0)) * 0.1;
    if( type == 2 ){
        if( 1.0 <= mod(pos.y,2.0) ){
            pos.x += 0.5 + m * move;
        }else{
            pos.x -= m * move;
        }
    }else if( type == 3 ){
        if( 1.0 <= mod(pos.y,2.0) ){
            pos.x += 0.5 + m * move;
        }
    }
    //*/
    
    return texture2D( tex, mod( pos, 1.0 ) ) * distance(vec2(0.5),gl_FragCoord.xy/resolution) / 0.09;
    
}

void main() {
    vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / resolution.y;
    gl_FragColor = trans(pos);
}