#version 120

// Textures
uniform sampler2D tex;
uniform float     time;
uniform vec2      resolution;
uniform float     value;

void main(void){
    
	vec2 texCoord = gl_FragCoord.xy / resolution;
    float cycle = mod( time / 10.0, 1.0 );
    
    texCoord.x = mod( ( texCoord.x + tan( (texCoord.y-cycle) * 3.0 * value ) * value / 20.0 ), 1.0 );
    
    gl_FragColor = texture2D( tex, texCoord );
    
}