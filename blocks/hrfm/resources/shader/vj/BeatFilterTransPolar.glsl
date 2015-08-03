#version 120

// Textures
uniform sampler2D tex;
uniform float     time;
uniform vec2      resolution;

uniform int   type;
uniform float bpm_position;

uniform float segment_x;
uniform float segment_y;
uniform float move;

vec4 trans(vec2 p)
{
    
    float theta = ( atan(p.y, p.x) / 6.283185307 + 0.25 ) * segment_x;
    float r = length(p) * segment_y;
    
    if( type == 1 ){
        theta += r * bpm_position * 0.3;
    }
    
    if( move != 0.0 ){
        if( 1.0 <= mod(r,2.0) ){
            theta += 0.5 + 1.0 * bpm_position;
        }else{
            theta -= 1.0 * bpm_position;
        }
    }
    
    vec2 pos = vec2(theta, r);
    
    return texture2D( tex, mod( pos, 1.0 ) );
    
}

void main() {
    vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / resolution.y;
    gl_FragColor = trans(pos);
}