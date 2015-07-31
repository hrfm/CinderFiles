#version 120

// Textures
uniform sampler2D tex;
uniform float time;
uniform vec2 resolution;

uniform float bpm_position;

void main(void){
    
    vec2 texCoord = gl_FragCoord.xy / resolution;
    vec4 color = texture2D( tex, texCoord.xy );
    
    //color *= sqrt( 1.0 - bpm_position * bpm_position );
    
    gl_FragColor = color;
    
}