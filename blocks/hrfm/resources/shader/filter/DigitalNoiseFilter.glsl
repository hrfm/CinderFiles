#version 120

// Textures
uniform sampler2D tex;
uniform float     time;
uniform vec2      resolution;
uniform float     value;
uniform int       vertical;

void main(void){
    
	vec2 texCoord = gl_FragCoord.xy / resolution;
    float cycle = mod( time / 10.0, 1.0 );
    
    if( vertical == 0 ){
        
        // --- ヨコノイズ
        float y = floor( texCoord.y / 0.02 ) * 0.02 + 0.2;
        
        float val = sin( sin( y ) * ( value * value ) * 100 );
        val = floor( val / (1.0-value) ) * 0.005;
        
        texCoord.x = mod( ( texCoord.x + val ), 1.0 );
        
    }else{
        
        // --- タテノイズ
        
        float x = floor( texCoord.x / 0.02 ) * 0.02 + 0.2;
        
        float val = sin( sin( x ) * ( value * value ) * 100 );
        val = floor( val / (1.0-value) ) * 0.001;
        
        texCoord.y = mod( ( texCoord.y + val ), 1.0 );
        
    }
    
    gl_FragColor = texture2D( tex, texCoord );
    
}