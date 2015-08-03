#version 120

// Textures
uniform sampler2D tex;
uniform float time;
uniform vec2 resolution;

uniform float bpm_position;

void main(void){
    vec2 texCoord = gl_FragCoord.xy / resolution;
    texCoord.y = sin(texCoord.x * 3.1415);
    vec4 color = texture2D( tex, texCoord.xy );
    gl_FragColor = color;
}