#version 120

// Textures
uniform sampler2D tex;
uniform float time;
uniform vec2 resolution;

uniform float bpm_position;
uniform float speed;
uniform float chroma;

float sigmoid( float val, float gain ){
    float v = val - 0.5;
    return 1.0 / ( 1.0 + exp( -gain * v ) );
}

void main(void){
    
    vec2 texCoord = gl_FragCoord.xy / resolution;
    vec4 color = texture2D( tex, texCoord.xy );
    color *= 1.2 * ( 0.5 + 0.5 * sigmoid( sin( texCoord.y * 800 ), 2 ) );
    
    vec3 tColor  = vec3(0.0);
    
    tColor.r = 0.3 + 0.7 * cos( time * speed * 3.1415 + texCoord.x * 5.0 );
    tColor.g = 0.3 + 0.7 * sin( time * speed * 3.1415 + texCoord.x * 5.0 );
    tColor.b = 0.3 + 0.7 * ( 1.0 - cos( time * speed * 3.1415 + texCoord.x * 5.0 ) );
    
    color.r += ( ( color.r * tColor.r ) - color.r ) * chroma;
    color.g += ( ( color.g * tColor.g ) - color.g ) * chroma;
    color.b += ( ( color.b * tColor.b ) - color.b ) * chroma;
    
    gl_FragColor = color;
    
}