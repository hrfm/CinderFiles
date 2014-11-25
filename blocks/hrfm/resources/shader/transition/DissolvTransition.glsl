#version 120

uniform sampler2D currentTex;
uniform sampler2D nextTex;
uniform vec2      resolution;
uniform float     progress;

void main(){
    
    // 各テクスチャ上の色を取得.
    vec2 pos = gl_FragCoord.xy / resolution;
    vec4 currentColor = texture2D( currentTex, pos );
    vec4 nextColor    = texture2D( nextTex, pos );
    
    // alpha の設定.
    
    currentColor.a *= 1.0 - progress;
    nextColor.a    *= progress;
    
    // rgb = 下層の面の色(rgb) * (1 - 上層のアルファ) + 上層の面の色(rgb) * 上層のアルファ
    // a   = 下層の面のアルファ + 上層の面のアルファ
    vec4 outputColor = vec4( vec3( currentColor.rgb * ( 1.0 - nextColor.a ) ) + vec3( nextColor.rgb * nextColor.a ), 1.0 );
    outputColor.a    = currentColor.a + nextColor.a;
    
    gl_FragColor.rgba = outputColor;
    
}