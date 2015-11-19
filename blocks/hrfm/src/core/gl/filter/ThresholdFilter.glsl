#version 150

// Textures
uniform sampler2D tex;
uniform float     time;
uniform vec2      resolution;
uniform float     strength;

out vec4 oColor;

void main(void){
	vec2 texCoord = gl_FragCoord.xy / resolution;
    texCoord.y = 1.0 - texCoord.y;
    
    vec4 color    = texture( tex, texCoord );
    
    // 明るさを計算し閾値以下の場合黒にする.
    float chroma = color.x * 0.3 + color.y * 0.59 + color.z * 0.11;
    if( chroma < strength ){
        color = vec4(0.0);
    }
    
    oColor = color;
    
}