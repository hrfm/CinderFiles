#version 120

// Textures
uniform sampler2D tex;
uniform float time;
uniform vec2 resolution;
uniform float segments;

void main(void){
    
    vec2 texCoord = gl_FragCoord.xy / resolution;
    
    vec4 color;
    
    // ---------------------------------------------------
    
    if( segments == 1 ){
        
        color = texture2D( tex, texCoord.xy );
        
    }else if( segments == 2 ){
        
        // 横2分割 その1
        texCoord.x *= 2.0;
        if( 1.0 <= texCoord.x ){
            texCoord.x = 1.0 - ( texCoord.x - 1.0 );
        }
        color = texture2D( tex, texCoord.xy );
        
    }else if( segments == 3 ){
        
        // 横2分割 その2
        texCoord.x *= 2.0;
        if( 1.0 <= texCoord.x ){
            texCoord.x -= 1.0;
        }else{
            texCoord.x = 1.0 - texCoord.x;
        }
        color = texture2D( tex, texCoord.xy );
        
    }else if( segments == 4 ){
        
        // 縦2分割 その1
        if( 0.5 < texCoord.y ){
            texCoord.x = 1.0 - texCoord.x;
        }
        texCoord.y = mod( texCoord.y * 2.0, 1.0 );
        color = texture2D( tex, texCoord.xy );
        
    }else if( segments == 5 ){
        
        // 縦2分割 その2
        if( 0.5 > texCoord.y ){
            texCoord.x = 1.0 - texCoord.x;
        }
        texCoord.y = mod( texCoord.y * 2.0, 1.0 );
        color = texture2D( tex, texCoord.xy );
        
    }else if( segments == 6 ){
        
        // 4分割
        texCoord.x = mod( texCoord.x * 2.0, 1.0 );
        texCoord.y = mod( texCoord.y * 2.0, 1.0 );
        color = texture2D( tex, texCoord.xy );
        
    }else{
        
        color = texture2D( tex, texCoord.xy );
        
    }
    
    
    
    /*
    texCoord.x *= 4;
    texCoord.y *= 4;
    
    vec2 p = mod( texCoord, 1.0 );
    
    float x = mod( texCoord.x, 4.0 );
    
    if( 3.0 < x ){
        p = vec2( 1.0 - p.y, p.x );
    }else if( 2.0 < x ){
        p = vec2( 1.0 - p.x, p.y );
    }else if( 1.0 < x ){
        p = vec2( p.y, p.x );
    }
    
    vec4 color = texture2D( tex, p.xy );
    
     
    */
    
    // ---------------------------------------------------
    
    gl_FragColor = color;
    
}