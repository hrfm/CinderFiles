#version 120

// Textures
uniform sampler2D tex;
uniform float time;
uniform vec2 resolution;

void main(void){
    
    vec2 texCoord = gl_FragCoord.xy / resolution;
    vec4 color    = texture2D( tex, texCoord );
    
    vec2 dist = vec2( texCoord.x - 0.5, texCoord.y - 0.5 );
    
    float len = length( dist );
    float f = 0.1;
    
    if( (0.5-f) < len ){
        color *= vec4( 1.0 - 1.0 * ((len-(0.5-f))/f) );
    }
    
    // ---------- Color.
    gl_FragColor = color;
    
}