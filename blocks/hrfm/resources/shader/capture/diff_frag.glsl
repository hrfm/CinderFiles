#version 150

in  vec4 Color;
out vec4 oColor;

// Textures
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform vec2 resolution;

void main( void ) {
    
    vec4 outputColor;
    
    vec2 texPos = gl_FragCoord.xy / resolution;
    vec4 tex0Color = texture( tex0, texPos );
    vec4 tex1Color = texture( tex1, texPos );
    
    outputColor.x = tex0Color.x - tex1Color.x;
    outputColor.y = tex0Color.y - tex1Color.y;
    outputColor.z = tex0Color.z - tex1Color.z;
    
    if( outputColor.x * outputColor.y * outputColor.z < 0.005 ){
        outputColor = vec4(0.0,0.0,0.0,1.0);
    }else{
        outputColor = vec4(1.0);
    }
    
    oColor = outputColor;
    
}