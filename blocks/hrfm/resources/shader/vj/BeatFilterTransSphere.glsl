#version 120

// Textures
uniform sampler2D tex;
uniform float time;
uniform vec2 resolution;

uniform float bpm_position;

vec4 trans(vec2 p)
{
    
    const float segment_x = 8.0;
    const float segment_y = 1.0;
    
    float tx = ( acos(-p.x/sqrt(1-p.y*p.y)) / 3.1415 );
    float ty = ( asin(-p.y/sqrt(1-p.x*p.x)) / 3.1415 + 0.5 );//asin(p.y) / 3.1415 + 0.5;
    float r = length(p);
    if( (r>0.0 && r<1.0) ){
        vec2 pos = vec2(tx, ty);
        return texture2D( tex, mod( pos, 1.0 ) );
    }else{
        return vec4(0.0);
    }
    
}

void main() {
    vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / resolution.y;
    gl_FragColor = trans(pos);
}