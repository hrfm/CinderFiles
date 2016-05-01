#version 400

// Textures
uniform sampler2D tex;
uniform vec2 resolution;

uniform sampler2D depthTex;
uniform float strength;

out vec4 oColor;

void main(void){
    
    vec2 texCoord = gl_FragCoord.xy / resolution;
    
    vec4 color = texture( tex, texCoord );
    vec4 depthColor = texture( depthTex, texCoord );
    
    float depthA = 1.0 - color.x;
    float depthB = color.x;
    
    oColor = vec4( vec3( color.xyz * depthA + depthColor.xyz * depthB ), 1.0 );
    
}