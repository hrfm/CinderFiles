#version 120

// Textures

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D captureTex;
uniform sampler2D diffTex;
uniform vec2 resolution;
uniform vec2 matrix;
uniform float time;

//
// 文字Matrix 用のフェードテクスチャを統合する Shader.
//

float sigmoid( float val ){
    return 1.0 / ( 1.0 + exp( -5 * ( -1 + val * 2 ) ) );
}

void main( void ) {
    
    // 最終アウトの色.
    vec4 outputColor;
    
    // ---
    
    // 各テクスチャ上の色を取得.
    vec2 pos = gl_FragCoord.xy / resolution;
    
    // 各 fade の色を設定する.
    //vec4 tex0col = texture2D( tex0, mod( pos * vec2( 3.0, 1.0 ), 1.0 ) );
    vec4 tex0col = texture2D( tex0, mod( pos, 1.0 ) );
    tex0col.xyz *= 0.8;
    vec4 tex1col = texture2D( tex1, pos );
    
    // キャプチャのテクスチャだけ 文字の領域と応用にややピクセルを荒く走査し グレースケールにする.
    vec2 capturePos = vec2( floor( pos.x * matrix.x ) / (matrix.x+1), floor( pos.y * matrix.y ) / matrix.y );
    vec4 captureTexCol = texture2D( captureTex, capturePos );
    float glayscale = ( 0.30*captureTexCol.x + 0.59*captureTexCol.y + 0.11*captureTexCol.z );
    
    vec4 diffTexCol = texture2D( diffTex, capturePos );
    float diffGlayscale = ( 0.30*diffTexCol.x + 0.59*diffTexCol.y + 0.11*diffTexCol.z );
    
    // --- シグモイド曲線で色曲線を曲げる.
    //glayscale = sigmoid( glayscale );
    //glayscale *= 0.85;
    
    if( glayscale < diffGlayscale ){
        glayscale = diffGlayscale;
    }
    
    if( tex1col.x > tex1col.y ){
        tex1col = vec4(tex1col.x);
    }else{
        tex1col.xyz += ( vec3( glayscale ) - tex1col.xyz ) * 0.7;
    }
    
    outputColor = tex0col;
    
    if( outputColor.x < tex1col.x ){
        outputColor = tex1col;
    }
    
    if( outputColor.x < glayscale ){
        outputColor = vec4( vec3(glayscale), 1.0 );
    }
    
    gl_FragColor = outputColor;
    
}