#version 120

// Textures
uniform sampler2D tex;
uniform float time;
uniform vec2 resolution;

void main(void){
    
    vec2 texCoord = gl_FragCoord.xy / resolution;
    
    float tx = cos(-texCoord.x/sqrt(1-texCoord.y*texCoord.y)) - 0.5;
    float ty = asin(texCoord.y) - 0.5;
    float r = length(texCoord);
    
    if( r>0.0 && r<1.0 ){
        texCoord = vec2(tx, ty)/2.0;
        vec4 color = texture2D( tex, texCoord.xy );
        gl_FragColor = color;
    }else{
        gl_FragColor = vec4(0.0);
    }
    
}