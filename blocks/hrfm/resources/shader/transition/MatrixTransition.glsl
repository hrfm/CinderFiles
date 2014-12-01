#version 120

uniform sampler2D currentTex;
uniform sampler2D nextTex;
uniform sampler2D matrixTex;
uniform vec2      segments; // Segments of fontMatrix.
uniform vec2      resolution;
uniform float     progress;

vec2 step(){
    return vec2( ( resolution.x / segments.x ) / resolution.x, ( resolution.y / segments.y ) / resolution.y );
}

vec2 block( vec2 pos ){
    vec2 step = step();
    return vec2( pos.x - mod( pos.x, step.x ) + step.x * 0.5, pos.y - mod( pos.y, step.y ) + step.y * 0.5 );
}

vec2 center(){
    vec2 step = step();
    return vec2( 0.5 - mod( 0.5, step.x ) + step.x * 0.5, 0.5 - mod( 0.5, step.y ) + step.y * 0.5 );
}

vec4 fromCenter( float threshold ){
    
    // 各テクスチャ上の色を取得.
    vec2 pos = gl_FragCoord.xy / resolution;
    
    vec4 currentColor = texture2D( currentTex, pos );
    vec4 nextColor    = texture2D( nextTex, pos );
    vec4 matrixColor  = texture2D( matrixTex, pos );
    
    vec2 step   = step();
    vec2 center = center();
    vec2 block  = block(pos);
    float dist  = distance( center, block );
    
    if( progress < threshold ){
        
        // progress が 0.0 から threshold の間の処理
        // 少しずつ文字 MATRIX に変換
        
        if( dist < progress / threshold ){
            
            // MATRIX
            pos.x = pos.x - mod( pos.x, step.x ) + step.x * 0.5;
            pos.y = pos.y - mod( pos.y, step.y ) + step.y * 0.5;
            return texture2D( nextTex, pos ) * matrixColor;
            
        }else{
            
            // 画像.
            return currentColor;
            
        }
        
    }else if( ( 1.0 - threshold ) < progress ){
        
        // progress が (1.0-threshold) から 1.0 の間の処理
        // 少しずつ文字 MATRIX から画像に変換
        
        if( dist < ( progress - ( 1.0 - threshold ) ) / threshold ){
            
            // 画像
            return nextColor;
            
        }else{
            
            // MATRIX
            pos.x = pos.x - mod( pos.x, step.x ) + step.x * 0.5;
            pos.y = pos.y - mod( pos.y, step.y ) + step.y * 0.5;
            return texture2D( nextTex, pos ) * matrixColor;
            
        }
        
    }else{
        
        // MATRIX
        pos.x = pos.x - mod( pos.x, step.x ) + step.x * 0.5;
        pos.y = pos.y - mod( pos.y, step.y ) + step.y * 0.5;
        return texture2D( nextTex, pos ) * matrixColor;
        
    }
    
}

vec4 lines( float threshold ){
    
    // 各テクスチャ上の色を取得.
    vec2 pos = gl_FragCoord.xy / resolution;
    
    vec4 currentColor = texture2D( currentTex, pos );
    vec4 nextColor    = texture2D( nextTex, pos );
    vec4 matrixColor  = texture2D( matrixTex, pos );
    
    vec2 step   = step();
    vec2 block  = block(pos);
    
    float lineX, lineY, lineZ;
    
    if( progress < threshold ){
        
        lineY = ( progress / threshold );
        if( pos.y < lineY - mod( lineY, step.y ) ){
            lineX = 1.0;
        }else{
            lineX = mod( lineY, step.y ) / step.y;
        }
        
        // progress が 0.0 から threshold の間の処理
        // 少しずつ文字 MATRIX に変換
        
        if( block.y < lineY && block.x < lineX ){
            
            // MATRIX
            pos.x = pos.x - mod( pos.x, step.x ) + step.x * 0.5;
            pos.y = pos.y - mod( pos.y, step.y ) + step.y * 0.5;
            return texture2D( nextTex, pos ) * matrixColor;
            
        }else{
            
            // 画像.
            return currentColor;
            
        }
        
    }else if( ( 1.0 - threshold ) < progress ){
        
        lineY = ( ( progress - ( 1.0 - threshold ) ) / threshold );
        if( pos.y < lineY - mod( lineY, step.y ) ){
            // 既に出力済みのline
            lineX = 1.0;
            lineZ = 1.0;
        }else{
            //
            lineX = mod( lineY, step.y ) / step.y;
            if( block.x < lineX ){
                lineZ = 1.0;
            }else{
                lineZ = mod( lineX, step.x ) / step.x;
            }
        }
        
        // progress が (1.0-threshold) から 1.0 の間の処理
        // 少しずつ文字 MATRIX から画像に変換
        
        if( block.y < lineY && block.x < lineX && lineZ == 1.0 ){
            
            // 画像
            return nextColor;
            
        }else{
            
            // MATRIX
            pos.x = pos.x - mod( pos.x, step.x ) + step.x * 0.5;
            pos.y = pos.y - mod( pos.y, step.y ) + step.y * 0.5;
            return texture2D( nextTex, pos ) * matrixColor;
            
        }
        
    }else{
        
        // MATRIX
        pos.x = pos.x - mod( pos.x, step.x ) + step.x * 0.5;
        pos.y = pos.y - mod( pos.y, step.y ) + step.y * 0.5;
        return texture2D( nextTex, pos ) * matrixColor;
        
    }
    
}

vec4 noise( float threshold ){
    
    // 各テクスチャ上の色を取得.
    vec2 pos = gl_FragCoord.xy / resolution;
    
    vec4 currentColor = texture2D( currentTex, pos );
    vec4 nextColor    = texture2D( nextTex, pos );
    vec4 matrixColor  = texture2D( matrixTex, pos );
    
    vec2 step   = step();
    vec2 block  = block(pos);
    
    float lineX, lineY, lineZ;
    
    if( progress < threshold ){
        
        lineY = ( progress / threshold );
        if( pos.y < lineY - mod( lineY, step.y ) ){
            lineX = 1.0;
        }else{
            lineX = mod( lineY, step.y ) / step.y;
        }
        
        // progress が 0.0 から threshold の間の処理
        // 少しずつ文字 MATRIX に変換
        
        if( block.y < lineY && block.x < lineX ){
            
            // MATRIX
            pos.x = pos.x - mod( pos.x, step.x ) + step.x * 0.5;
            pos.y = pos.y - mod( pos.y, step.y ) + step.y * 0.5;
            
            pos.x = mod( ( pos.x + tan( (pos.y-progress) * 3.0 * 1.0 ) * 1.0 / 20.0 ), 1.0 );
            
            return texture2D( nextTex, pos ) * matrixColor;
            
        }else{
            
            // 画像.
            return currentColor;
            
        }
        
    }else if( ( 1.0 - threshold ) < progress ){
        
        pos.x = mod( ( pos.x + tan( (pos.y-progress) * 3.0 * progress ) * progress / 20.0 ), 1.0 );
        
        return texture2D( nextTex, pos );
        
    }else{
        
        // MATRIX
        pos.x = pos.x - mod( pos.x, step.x ) + step.x * 0.5;
        pos.y = pos.y - mod( pos.y, step.y ) + step.y * 0.5;
        
        pos.x = mod( ( pos.x + tan( (pos.y-progress) * 3.0 * 1.0 ) * 1.0 / 20.0 ), 1.0 );
        
        return texture2D( nextTex, pos ) * matrixColor;
        
    }
    
}

void main(){
    vec4 color = noise(0.5);
    gl_FragColor = color;
}
