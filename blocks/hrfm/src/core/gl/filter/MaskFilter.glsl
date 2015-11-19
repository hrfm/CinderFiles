#version 120

// Textures
uniform sampler2D tex;
uniform vec2 resolution;

uniform sampler2D maskTex;
uniform float strength;

out vec4 oColor;

void main(void){
    
	vec2 texCoord = gl_FragCoord.xy / resolution;
    texCoord.y = 1.0 - texCoord.y;
    
    vec4 color = texture2D( tex, texCoord );
    vec4 maskColor = texture2D( maskTex, texCoord );
    
    oColor = color + ( color*maskColor - color ) * strength;
    
}