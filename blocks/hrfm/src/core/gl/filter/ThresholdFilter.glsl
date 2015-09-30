#version 120

// Textures
uniform sampler2D tex;
uniform float     time;
uniform vec2      resolution;
uniform float     strength;

void main(void){
	vec2 texCoord = gl_FragCoord.xy / resolution;
    vec4 color    = texture2D( tex, texCoord );
    // 明るさを計算し閾値以下の場合黒にする.
    float chroma = color.x * 0.3 + color.y * 0.59 + color.z * 0.11;
    if( chroma < strength ){
        color = vec4(0.0);
    }
    gl_FragColor = color;
}