#version 400

// Textures
uniform sampler2D tex;
uniform float     time;
uniform vec2      resolution;
uniform float     strength;

in  vec4 Color;
out vec4 oColor;

void main(void){
    
	vec2 texCoord = gl_FragCoord.xy / resolution;
    
    vec4 color    = texture( tex, texCoord );
    
    // --- 白黒っぽく
    float chroma = color.x * 0.3 + color.y * 0.59 + color.z * 0.11; // 明るさを計算
    color.xyz = color.xyz + ( vec3(chroma) - color.xyz ) * 0.8;
    
    // --- 青っぽく変換
    color.x += ( color.x*0.7 - color.x ) * strength;
    color.y += ( color.y*1.2 - color.y ) * strength;
    color.z += ( color.z*1.6 - color.y ) * strength;
    
    // ---------- Color.
    oColor = color;
    
}