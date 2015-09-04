#version 120

// Textures
uniform sampler2D tex;
uniform vec2  resolution;
uniform float cols;
uniform float rows;
uniform vec4  cliprect;

void main(void){
    
	vec2 texCoord = gl_FragCoord.xy / resolution;
    texCoord.x = mod( texCoord.x * cols, 1.0 );
    texCoord.y = mod( texCoord.y * rows, 1.0 );
    
    texCoord.x = cliprect.x + ( cliprect.z - cliprect.x ) * texCoord.x;
    texCoord.y = cliprect.y + ( cliprect.w - cliprect.y ) * texCoord.y;
    
    vec4 color = texture2D( tex, texCoord );
    
    gl_FragColor = color;
    
}