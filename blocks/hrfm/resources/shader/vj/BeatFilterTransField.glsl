#version 120

// Textures
uniform sampler2D tex;
uniform float time;
uniform vec2 resolution;

uniform float bpm_position;
uniform int vertical;
uniform float move;

vec4 trans(vec2 p)
{
    
    const float segment_x = 2.0;
    const float segment_y = 0.8;
    const float height = 0.5;
    
    float r;
    vec2 pos;
    vec4 color;
    float brightness;
    
    if( vertical == 1 ){
        
        r = height/p.x * segment_y;
        pos = vec2(p.y*r, r);
        
        if( mod( pos.y, 2.0 ) < 1.0 ){
            pos.x += time * move;
        }else{
            pos.x -= time * move;
        }
        
        color = texture2D( tex, mod( pos, 1.0 ) );
        brightness = sqrt( min(1.0,abs( 0.5 - (gl_FragCoord.x/resolution.x) ) / 0.05) );
        
    }else{
        
        r = height/p.y * segment_y;
        pos = vec2(p.x*r, r);
        
        if( mod( pos.y, 2.0 ) < 1.0 ){
            pos.x += time * move;
        }else{
            pos.x -= time * move;
        }
        
        color = texture2D( tex, mod( pos, 1.0 ) );
        brightness = sqrt( min(1.0,abs( 0.5 - (gl_FragCoord.y/resolution.y) ) / 0.05) );
        
    }
    
    return color * brightness;
    
}

void main() {
    vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / resolution.y;
    gl_FragColor = trans(pos);
}