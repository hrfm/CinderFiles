#version 120

// Textures
uniform sampler2D tex;
uniform vec2 resolution;
uniform int cols;
uniform int rows;

void main(void){
	vec2 texCoord = gl_FragCoord.xy / resolution;
    texCoord.x = mod( texCoord.x * 3.0, 1.0 );
    texCoord.y = mod( texCoord.y * 3.0, 1.0 );
    gl_FragColor = texture2D( tex, texCoord );
}