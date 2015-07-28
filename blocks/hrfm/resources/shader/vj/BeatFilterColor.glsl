#version 120

// Textures
uniform sampler2D tex;
uniform float time;
uniform vec2 resolution;

uniform float bpm_position;

void main(void){
    
    vec2 texCoord = gl_FragCoord.xy / resolution;
    vec4 color = texture2D( tex, texCoord.xy );
    
    //color *= max(0.0, 1.0 - abs( texCoord.y - bpm_position ) / 0.8);
    
    gl_FragColor = color;
    
}