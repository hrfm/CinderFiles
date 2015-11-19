#version 150

// Textures
uniform sampler2D tex;
uniform vec2 resolution;
uniform float scrollX;
uniform float scrollY;

out vec4 oColor;

void main(void){
    
	vec2 texCoord = gl_FragCoord.xy / resolution;
    texCoord.y = 1.0 - texCoord.y;
    
    // --- 縦方向のループ.
    texCoord.x = mod( ( texCoord.x + scrollX ), 1.0 );
    texCoord.y = mod( ( texCoord.y + scrollY ), 1.0 );
    
    // ---------- Color.
    oColor = texture( tex, texCoord );
    
}