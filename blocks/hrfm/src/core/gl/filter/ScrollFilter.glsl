#version 120

// Textures
uniform sampler2D tex;
uniform vec2 resolution;
uniform float scrollX;
uniform float scrollY;

void main(void){
    
	vec2 texCoord = gl_FragCoord.xy / resolution;
    
    // --- 縦方向のループ.
    texCoord.x = mod( ( texCoord.x + scrollX ), 1.0 );
    texCoord.y = mod( ( texCoord.y + scrollY ), 1.0 );
    
    // ---------- Color.
    gl_FragColor = texture2D( tex, texCoord );
    
}