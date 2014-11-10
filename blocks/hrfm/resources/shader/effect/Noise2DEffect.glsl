#version 120

// Textures
uniform sampler2D tex;
uniform float time;
uniform vec2 resolution;
uniform float value;

uniform int vertical;
uniform int analog;

void main(void){
    
	vec2 texCoord = gl_FragCoord.xy / resolution;
    float cycle = mod( time / 10.0, 1.0 );
    
    if( analog == 1){
        
        // --- ヨコノイズ
        
        texCoord.x = mod( ( texCoord.x + tan( (texCoord.y-cycle) * 3.0 * value ) * value / 20.0 ), 1.0 );
        
    }else{
        
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
        
    }
    
    // --- ノイズ
    //texCoord.y = mod( ( texCoord.x + atan( cycle + texCoord.y * 10) * value/10 ), 1.0 );
     
    // ---------- Color.
    
    vec4 color = texture2D( tex, texCoord );
    
    /*
    gl_FragColor.rgb = HSVtoRGB(vec3(gl_TexCoord[0].x * 360.0, gl_TexCoord[0].y, 1.0));
    gl_FragColor.a = 1.0;
    gl_FragColor *= color;
    /*/
    gl_FragColor = color;
    //*/
    
}