#version 150

out vec4 Color;
out vec3 normal;

uniform sampler2D tex;
uniform float     time;
uniform vec2      resolution;

void main(void){
    /*
    gl_FragColor = vColor;
    /*/
    vec2 texCoord = gl_FragCoord.xy / resolution;
    vec4 color = texture2D( tex, texCoord );
    gl_FragColor = color;
    //*/
}