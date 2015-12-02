#version 400

in  vec4 Color;
out vec4 oColor;

// Textures
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform vec2 resolution;

void main( void ) {
    
    vec2 texPos = gl_FragCoord.xy / resolution;
    
    vec4 outputColor = vec4( 0.0, 0.0, 0.0, 1.0 );
    outputColor.xyz = texture( tex0, texPos ).xyz - texture( tex1, texPos ).xyz;
    if( outputColor.x * outputColor.y * outputColor.z < 0.005 ){
        outputColor = vec4(0.0,0.0,0.0,1.0);
    }else{
        outputColor = vec4(1.0);
    }
    
    oColor = outputColor;
    
}