#version 400

// Textures
uniform sampler2D tex;
uniform float     time;
uniform vec2      resolution;
uniform float     strength;
uniform int       vertical;

out vec4 oColor;

void main(void){
    
	vec2 texCoord = gl_FragCoord.xy / resolution;
    texCoord.y = 1.0 - texCoord.y;
    
    float cycle = mod( time / 10.0, 1.0 );
    
    if( vertical == 0 ){
        
        // --- ヨコノイズ
        float y = floor( texCoord.y / 0.002 ) * 0.002 + 0.2;
        
        float val = sin( sin( y ) * ( strength * strength ) * 100 );
        val = floor( val / (1.0-strength) ) * 0.005;
        
        texCoord.x = mod( ( texCoord.x + val ), 1.0 );
        
    }else{
        
        // --- タテノイズ
        
        float x = floor( texCoord.x / 0.002 ) * 0.002 + 0.2;
        
        float val = sin( sin( x ) * ( strength * strength ) * 100 );
        val = floor( val / (1.0-strength) ) * 0.001;
        
        texCoord.y = mod( ( texCoord.y + val ), 1.0 );
        
    }
    
    oColor = texture( tex, texCoord );
    
}