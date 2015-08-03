#version 120

// Textures
uniform sampler2D tex;
uniform float time;
uniform vec2 resolution;

uniform float bpm_position;

uniform float param;

void main() {
    
    vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / resolution;
    
    float param1 = 0.2;
    float param2 = 0.1;
    float param3 = 0.5;
    float param4 = 0.0;
    
    float rot = 3.1415926535 * (time*0.3+param4) ;
    float sr = sin(rot) ;
    float cr = cos(rot) ;
    
    mat2 tr = mat2(
                   cr,-sr,
                   sr,cr
    );
    
    float px = abs(p.x);
    float py = abs(p.y);
    if(px<py) {
        float t = py ;
        py = px ;
        px = t ;
    }
    
    float mag = 1.0-param1 ;
    float ox = param2 ;
    float oy = param3 ;
    
    vec2 tp = tr * vec2( ox + px * mag, oy + py * mag ) ;
    
    gl_FragColor = texture2D(tex,mod(tp,1.0)) ;
    
}