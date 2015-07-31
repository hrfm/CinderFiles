#version 120

// Textures
uniform sampler2D tex;
uniform float time;
uniform vec2 resolution;

uniform float bpm_position;
uniform int   has_direction;

uniform vec2 segments;
uniform int  rotate;

void main(void){
    
    vec2 texCoord = gl_FragCoord.xy / resolution;
    
    // ---------------------------------------------------
    
    if( has_direction == 0 ){
        
        texCoord.x *= segments.x;
        texCoord.y *= segments.y;
        texCoord = mod( texCoord, 1.0 );
        
    }else{
        
        if( segments.x == 1.0 && segments.y == 1.0 ){
            
            // 分割なしの時. 回転を適用
            if( rotate == 1 ){
                texCoord = vec2( texCoord.y, texCoord.x );
            }else if( rotate == 2 ){
                texCoord = vec2( 1.0 - texCoord.x, texCoord.y );
            }else if( rotate == 3 ){
                texCoord = vec2( 1.0 - texCoord.y, texCoord.x );
            }
            
        }else if( segments.x == 2.0 && segments.y == 2.0 ){
            
            // 4分割の時
            
            texCoord.x *= segments.x;
            texCoord.y *= segments.y;
            
            if( rotate == 0 ){
                
                if( 1.0 <= mod( texCoord.x, 2.0 ) ){
                    if( 1.0 <= mod( texCoord.y, 2.0 ) ){
                        texCoord = vec2( 1.0 - mod( texCoord.x, 1.0 ), 1.0 - mod( texCoord.y, 1.0 ) );
                    }else{
                        texCoord = vec2( mod( texCoord.y, 1.0 ), mod( texCoord.x, 1.0 ) );
                    }
                }else{
                    if( 1.0 <= mod( texCoord.y, 2.0 ) ){
                        texCoord = vec2( 1.0 - mod( texCoord.y, 1.0 ), mod( texCoord.x, 1.0 ) );
                    }else{
                        texCoord = vec2( mod( texCoord.x, 1.0 ), mod( texCoord.y, 1.0 ) );
                    }
                }
                
            }else if( rotate == 1 ){
                
                if( 1.0 <= mod( texCoord.x, 2.0 ) ){
                    if( 1.0 <= mod( texCoord.y, 2.0 ) ){
                        texCoord = vec2( mod( texCoord.y, 1.0 ), mod( texCoord.x, 1.0 ) );
                    }else{
                        texCoord = vec2( mod( texCoord.x, 1.0 ), mod( texCoord.y, 1.0 ) );
                    }
                }else{
                    if( 1.0 <= mod( texCoord.y, 2.0 ) ){
                        texCoord = vec2( 1.0 - mod( texCoord.x, 1.0 ), mod( texCoord.y, 1.0 ) );
                    }else{
                        texCoord = vec2( 1.0 - mod( texCoord.y, 1.0 ), mod( texCoord.x, 1.0 ) );
                    }
                }
                
            }else if( rotate == 2 ){
                
                if( 1.0 <= mod( texCoord.x, 2.0 ) ){
                    if( 1.0 <= mod( texCoord.y, 2.0 ) ){
                        texCoord = vec2( mod( texCoord.x, 1.0 ), mod( texCoord.y, 1.0 ) );
                    }else{
                        texCoord = vec2( 1.0 - mod( texCoord.y, 1.0 ), 1.0 - mod( texCoord.x, 1.0 ) );
                    }
                }else{
                    if( 1.0 <= mod( texCoord.y, 2.0 ) ){
                        texCoord = vec2( mod( texCoord.y, 1.0 ), 1.0 - mod( texCoord.x, 1.0 ) );
                    }else{
                        texCoord = vec2( 1.0 - mod( texCoord.x, 1.0 ), 1.0 - mod( texCoord.y, 1.0 ) );
                    }
                }
                
            }else if( rotate == 3 ){
                
                if( 1.0 <= mod( texCoord.x, 2.0 ) ){
                    if( 1.0 <= mod( texCoord.y, 2.0 ) ){
                        texCoord = vec2( 1.0 - mod( texCoord.y, 1.0 ), 1.0 - mod( texCoord.x, 1.0 ) );
                    }else{
                        texCoord = vec2( 1.0 - mod( texCoord.x, 1.0 ), 1.0 - mod( texCoord.y, 1.0 ) );
                    }
                }else{
                    if( 1.0 <= mod( texCoord.y, 2.0 ) ){
                        texCoord = vec2( mod( texCoord.x, 1.0 ), 1.0 - mod( texCoord.y, 1.0 ) );
                    }else{
                        texCoord = vec2( mod( texCoord.y, 1.0 ), 1.0 - mod( texCoord.x, 1.0 ) );
                    }
                }
                
            }else{
                
                texCoord = mod( texCoord, 1.0 );
                
            }
            
            
        }else{
            
            // 縦か横が2分割の時
            
            texCoord.x *= segments.x;
            texCoord.y *= segments.y;
            
            if( rotate == 0 ){
                
                // 通常のミラーリング
                
                if( 1.0 <= mod( texCoord.x, 2.0 ) || 1.0 <= mod( texCoord.y, 2.0 ) ){
                    texCoord.x = 1.0 - mod( texCoord.x, 1.0 );
                }else{
                    texCoord.x = mod( texCoord.x, 1.0 );
                }
                
                if( 1.0 <= mod( texCoord.y, 2.0 ) ){
                    texCoord.y = 1.0 - mod( texCoord.y, 1.0 );
                }else{
                    texCoord.y = mod( texCoord.y, 1.0 );
                }
                
            }else if( rotate == 1 ){
                
                if( 1.0 <= mod( texCoord.x, 2.0 ) || 1.0 <= mod( texCoord.y, 2.0 ) ){
                    texCoord.x = mod( texCoord.y, 1.0 );
                }else{
                    texCoord.x = 1.0 - mod( texCoord.y, 1.0 );
                }
                
                if( 1.0 <= mod( texCoord.y, 2.0 ) ){
                    texCoord.y = 1.0 - mod( texCoord.y, 1.0 );
                }else{
                    texCoord.y = mod( texCoord.y, 1.0 );
                }
                
            }else if( rotate == 2 ){
                
                // 逆ミラーリング
                
                if( 1.0 <= mod( texCoord.x, 2.0 ) || 1.0 <= mod( texCoord.y, 2.0 ) ){
                    texCoord.x = mod( texCoord.x, 1.0 );
                }else{
                    texCoord.x = 1.0 - mod( texCoord.x, 1.0 );
                }
                
                if( 1.0 <= mod( texCoord.y, 2.0 ) ){
                    texCoord.y = mod( texCoord.y, 1.0 );
                }else{
                    texCoord.y = 1.0 - mod( texCoord.y, 1.0 );
                }
                
            }else if( rotate == 3 ){
                
                if( 1.0 <= mod( texCoord.x, 2.0 ) || 1.0 <= mod( texCoord.y, 2.0 ) ){
                    texCoord.x = 1.0 - mod( texCoord.y, 1.0 );
                }else{
                    texCoord.x = mod( texCoord.y, 1.0 );
                }
                
                if( 1.0 <= mod( texCoord.y, 2.0 ) ){
                    texCoord.y = 1.0 - mod( texCoord.y, 1.0 );
                }else{
                    texCoord.y = mod( texCoord.y, 1.0 );
                }
                
            }else{
                
                texCoord = mod( texCoord, 1.0 );
                
            }
            
        }
        
    }
    
    // ---------------------------------------------------
    
    gl_FragColor = texture2D( tex, texCoord.xy );
    
}