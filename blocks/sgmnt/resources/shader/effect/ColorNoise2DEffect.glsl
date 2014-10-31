#version 120

// Textures
uniform sampler2D tex;
uniform float time;
uniform vec2 resolution;
uniform float value;

void main(void){
    
	vec2 texCoord = gl_FragCoord.xy / resolution;
    vec2 texCoord2 = texCoord;
    
    vec4 color = texture2D( tex, texCoord );
    
    texCoord2.x = texCoord.x + value / 10;
    color.x = texture2D( tex, texCoord2 ).x;
    
    texCoord2.x = texCoord.x - value / 10;
    color.z = texture2D( tex, texCoord2 ).z;
    
    // ---------- Color.
    gl_FragColor = color;
    
}