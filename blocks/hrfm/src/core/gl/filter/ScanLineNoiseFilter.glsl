#version 400

// Textures
uniform sampler2D tex;
uniform float time;
uniform vec2 resolution;

out vec4 oColor;

void main(void){
    
    vec2 texCoord = gl_FragCoord.xy / resolution;
    texCoord.y = 1.0 - texCoord.y;
    
    vec2 texCoord2 = texCoord;
    
    float cycle = mod( time / 10.0, 1.0 );
    
    vec4 color = texture( tex, texCoord );
    
    // --- 走査線を追加
    
    float size    = 300;
    vec2 position = texCoord2 * resolution;
    float height  = resolution.y + size;
    float posY    = - size + ( height * cycle );
    
    if( posY <= position.y && position.y < posY + size ){
        color.xyz *= 0.7;
    }
    
    color.xyz *= vec3( ( 1.0 + cos(sin(gl_FragCoord.y) )) / 2.0 );
    
    // ---------- Color.
    
    oColor = color;
    
}