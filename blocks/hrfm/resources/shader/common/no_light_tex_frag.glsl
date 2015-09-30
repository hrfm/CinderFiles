#version 120

varying vec4 vColor;
varying vec3 normal;
uniform sampler2D tex;
uniform float alpha;

void main(void){
    gl_FragColor = texture2D( tex, gl_TexCoord[0].xy );
    gl_FragColor.a = alpha;
}