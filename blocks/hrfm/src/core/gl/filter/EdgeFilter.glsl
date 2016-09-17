#version 400

uniform sampler2D tex;
uniform vec2      resolution;

uniform float     strength;

out vec4 oColor;

void main(){
    
    vec4 sum = vec4( 0.0, 0.0, 0.0, 0.0 );
    
    // 各テクスチャ上の色を取得.
    vec2 pos = gl_FragCoord.xy / resolution;
    
    //vec4 color = texture2D( tex, pos );
    
    float dX = ( 4.0 * strength ) / resolution.x;
    float dY = ( 4.0 * strength ) / resolution.y;
    
    vec3 color = vec3(0.5, 0.5, 0.5);
    
    float bias = 0.0;
    
    color += texture( tex, pos).xyz * -8.0;
    
    color += texture( tex, pos + vec2( -dX*bias, -dY*bias)).xyz;
    color += texture( tex, pos + vec2( 0.0, -dY)).xyz;
    color += texture( tex, pos + vec2(  dX*bias, -dY*bias)).xyz;
    
    color += texture( tex, pos + vec2( -dX, 0.0)).xyz;
    color += texture( tex, pos + vec2(  dX, 0.0)).xyz;
    
    color += texture( tex, pos + vec2( -dX*bias, dY*bias)).xyz;
    color += texture( tex, pos + vec2( 0.0, dY)).xyz;
    color += texture( tex, pos + vec2(  dX*bias, dY*bias)).xyz;
    
    color = step(0.55, color);
    float chroma = color.x * 0.3 + color.y * 0.59 + color.z * 0.11; // 明るさを計算
    
    if( 0.2 < chroma ){
        oColor = vec4( 1.0 );
    }else{
        oColor = vec4( vec3(0.0), 1.0);
    }
    
    //gl_FragColor = vec4(color, 1.0);
    
}