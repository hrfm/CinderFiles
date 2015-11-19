#version 120

// Textures
uniform sampler2D tex;
uniform float     time;
uniform vec2      resolution;
uniform float     strength;
uniform vec2      matrix;

uniform int useTexture;
uniform sampler2D maskTex;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec2 stepOfMtx(){
    return vec2( ( resolution.x / matrix.x ) / resolution.x, ( resolution.y / matrix.y ) / resolution.y );
}

void main(void){
    
    vec2 posOrigin  = gl_FragCoord.xy / resolution;
    posOrigin.y = 1.0 - posOrigin.y;

    // - matrix 上の所属座標を調べる
    vec2 stepOfMtx = stepOfMtx();
    vec2 posOfMtx  = gl_FragCoord.xy / resolution;
    posOfMtx.y = 1.0 - posOfMtx.y;

    posOfMtx.x = posOfMtx.x - mod( posOfMtx.x, stepOfMtx.x ) + stepOfMtx.x * 0.5;
    posOfMtx.y = posOfMtx.y - mod( posOfMtx.y, stepOfMtx.y ) + stepOfMtx.y * 0.5;
    
    // - matrix 上の所属座標の色を取得する
    vec4 color   = texture2D( tex, posOfMtx );
    
    if( useTexture == 1 ){
        
        //color = texture2D( maskTex, mod( posOrigin/stepOfMtx, 1.0 ) );
        color *= texture2D( maskTex, mod( posOrigin/stepOfMtx, 1.0 ) );
        
    }else{
        
        float dist = 1.0 - length(posOfMtx-posOrigin) / (stepOfMtx.x/2.0);
        if( dist < 0.0 ) dist = 0.0;
        
        color.rgba *= dist * rand(vec2(time));
        
    }
    
    // - 色を取得する.
    
    gl_FragColor = color;
    
}