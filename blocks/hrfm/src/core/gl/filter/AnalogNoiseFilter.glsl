#version 400

// Textures
uniform sampler2D tex;
uniform float     time;
uniform vec2      resolution;
uniform float     strength;

out vec4 oColor;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453 * time);
}

void main(void){
    
    //*
	vec2 texCoord = gl_FragCoord.xy / resolution;
    
    float cycle = mod( time / 300.0, 1.0 );
    
    float tanValue = tan( (texCoord.y-cycle) * 10.0 * strength * tan(cycle * strength) ) * strength / 100.0;
    float sinValue = sin( (texCoord.y-cycle) * strength * 1000.0 * sin(cycle*1000) ) / 200.0;
    
    texCoord.x = mod( texCoord.x + tanValue + sinValue, 1.0 );
    
    oColor = texture( tex, texCoord );// * rand( gl_FragCoord.xy );
    /*/
     
    // ULTRA2015 の時にこっちを使った。
    
    vec2 texCoord = gl_FragCoord.xy / resolution;
    float cycle = mod( time / 10.0, 1.0 );
    
    texCoord.x = mod( ( texCoord.x + tan( (texCoord.y-cycle) * 3.0 * strength ) * strength / 20.0 ), 1.0 );
    
    gl_FragColor = texture2D( tex, texCoord );

    //*/
    
}