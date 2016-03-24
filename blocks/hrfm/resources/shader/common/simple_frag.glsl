#version 400

out vec3 normal;
uniform sampler2D tex;
uniform float     time;
uniform vec2      resolution;

in  vec4 vColor;
out vec4 oColor;

void main(void){
    vec2 texCoord = gl_FragCoord.xy / resolution;
    vec4 color = texture( tex, texCoord );
    oColor = color;
}