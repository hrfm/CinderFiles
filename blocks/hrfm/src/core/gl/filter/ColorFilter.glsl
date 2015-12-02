#version 400

// Textures
uniform sampler2D tex;
uniform float time;
uniform vec2 resolution;
uniform vec3 color;

out vec4 oColor;

void main(void){
    
    vec2 texCoord = gl_FragCoord.xy / resolution;
    
    vec4 texColor = texture( tex, texCoord );
    
    if( color.r == 0 && color.g == 0 && color.b == 0 ){
        float chroma = texColor.x * 0.3 + texColor.y * 0.59 + texColor.z * 0.11;
        texColor = vec4( vec3(chroma), 1.0 );
    }else{
        texColor.r *= color.x;
        texColor.g *= color.y;
        texColor.b *= color.z;
    }
    
    oColor = texColor;
    
}