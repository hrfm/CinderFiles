#version 120

// Textures
uniform sampler2D tex;
uniform float     time;
uniform vec2      resolution;
uniform float     strength;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453 * time);
}

void main(void){
    
    /*
	vec2 texCoord = gl_FragCoord.xy / resolution;
    float cycle = mod( time / 300.0, 1.0 );
    
    float tanValue = tan( (texCoord.y-cycle) * 100.0 * strength * tan(cycle * strength) ) * strength / 20.0;
    float sinValue = 0;//sin( (texCoord.y-cycle) * strength * 1000.0 * sin(cycle*1000) ) / 100.0;
    
    texCoord.x = mod( texCoord.x + tanValue + sinValue, 1.0 );
    
    gl_FragColor = texture2D( tex, texCoord );// * rand( gl_FragCoord.xy );
    //*/
    
    vec2 texCoord = gl_FragCoord.xy / resolution;
    float cycle = mod( time / 10.0, 1.0 );
    
    texCoord.x = mod( ( texCoord.x + tan( (texCoord.y-cycle) * 3.0 * strength ) * strength / 20.0 ), 1.0 );
    
    gl_FragColor = texture2D( tex, texCoord );

     
    
}