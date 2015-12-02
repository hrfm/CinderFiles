#version 400

// Textures
uniform sampler2D tex;
uniform float     time;
uniform vec2      resolution;
uniform float     strength;

out vec4 oColor;

void main(void){
    
	vec2 texCoord = gl_FragCoord.xy / resolution;
    vec2 texCoord2 = texCoord;
    
    vec4 tmpColor;
    vec4 color = texture( tex, texCoord );
    
    //*
    
    texCoord2.x = texCoord.x + strength / 10;
    color.x = texture( tex, texCoord2 ).x;
    
    texCoord2.x = texCoord.x - strength / 10;
    color.z = texture( tex, texCoord2 ).z;
    /*/
    
    texCoord2.x = texCoord.x + strength / 10;
    tmpColor = texture2D( tex, texCoord2 );
    color += tmpColor * 0.8;
    
    texCoord2.x = texCoord.x - strength / 10;
    tmpColor = texture2D( tex, texCoord2 );
    color += tmpColor * 0.8;
    
    //*/
    
    // ---------- Color.
    oColor = color;
    
}