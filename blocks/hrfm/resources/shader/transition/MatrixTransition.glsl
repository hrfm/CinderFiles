#version 120

uniform sampler2D currentTex;
uniform sampler2D nextTex;
uniform sampler2D matrixTex;
uniform vec2      segments; // Segments of fontMatrix.
uniform vec2      resolution;
uniform float     progress;

void main(){
    
    // 各テクスチャ上の色を取得.
    vec2 pos = gl_FragCoord.xy / resolution;
    
    vec4 currentColor = texture2D( currentTex, pos );
    vec4 nextColor    = texture2D( nextTex, pos );
    vec4 matrixColor  = texture2D( matrixTex, pos );
    
    float dist = distance( vec2(0.5,0.5), pos );
    
    if( progress < 0.5 ){
        if( dist < progress / 0.5 ){
            nextColor.a = matrixColor.r;
            gl_FragColor.rgba = nextColor;
        }else{
            gl_FragColor.rgba = currentColor;
        }
    }else{
        if( dist < (progress-0.5) / 0.5 ){
            gl_FragColor.rgba = nextColor;
        }else{
            nextColor.a = matrixColor.r;
            gl_FragColor.rgba = nextColor;
        }
    }
    
    return;
    
    
    
    
    
    
    
    
    
    
    
    
    if( 0 < matrixColor.r ){
        
        // alpha の設定.
        
        currentColor.a *= 1.0 - progress;
        nextColor.a    *= progress;
        
        // rgb = 下層の面の色(rgb) * (1 - 上層のアルファ) + 上層の面の色(rgb) * 上層のアルファ
        // a   = 下層の面のアルファ + 上層の面のアルファ
        vec4 outputColor = vec4( vec3( currentColor.rgb * ( 1.0 - nextColor.a ) ) + vec3( nextColor.rgb * nextColor.a ), 1.0 );
        outputColor.a    = currentColor.a + nextColor.a;
        
        gl_FragColor.rgba = outputColor;
        
    }else{
        
        gl_FragColor.rgba = vec4(0.0);
        
    }
    
}