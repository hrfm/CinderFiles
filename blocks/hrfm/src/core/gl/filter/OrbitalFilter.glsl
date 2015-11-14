#version 150

// Textures
uniform sampler2D tex;
uniform sampler2D tex_speed;

uniform float time;

uniform vec2 windowSize;
uniform vec2 resolution;

uniform float r_scale;
uniform float r_min;
uniform float r_max;

out vec4 oColor;

vec2 trans(vec2 p){
    float theta = atan(p.y, p.x);
    float r = length(p);
    
    r *= r_scale;
    if( r < r_min || r_max < r  ){
        return vec2(-100.0);
    }
    
    vec4 speed = texture( tex_speed, mod( vec2( theta / (3.1415926535897932384626433*2), r ), 1.0 ) );
    theta += sin(speed.x*6.28) * time/ 30;
    
    return vec2( theta / (3.1415926535897932384626433*2), r );
    
}

float sigmoid( float val, float gain ){
    float v = val - 0.5;
    return 1.0 / ( 1.0 + exp( -gain * v ) );
}

void main(void){
    
    vec2 pos = ( ( gl_FragCoord.xy / windowSize ) * 2.0 ) - ( 0.5 * 2.0 );
    vec2 v   = trans(pos);
    
    if( v.x == -100 ){
        
        oColor = vec4( vec3(0.0), 0.1 );
        
    }else{
        
        vec4 color = texture( tex, mod(v,1.0) );
        
        color.a = 1.0 - ( v.y / r_max );
        if( color.a < 0.0 ){
            color.a = 0.0;
        }
        
        color.a = sigmoid( color.a, 20 );
        
        oColor = color;
        
    }
    
}