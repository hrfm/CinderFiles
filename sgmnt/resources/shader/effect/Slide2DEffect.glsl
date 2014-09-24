#version 120

// Textures
uniform sampler2D tex;
uniform vec2 resolution;
uniform float slideX;
uniform float slideY;

void main(void){
    
	vec2 texCoord = gl_FragCoord.xy / resolution;
    
    // --- 縦方向のループ.
    texCoord.x = mod( ( texCoord.x + slideX ), 1.0 );
    texCoord.y = mod( ( texCoord.y + slideY ), 1.0 );
    
    // ---------- Color.
    gl_FragColor = texture2D( tex, texCoord );
    
}