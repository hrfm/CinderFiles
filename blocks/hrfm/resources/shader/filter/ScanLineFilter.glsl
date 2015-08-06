#version 120

// Textures
uniform sampler2D tex;
uniform float time;
uniform vec2 resolution;

float sigmoid( float val, float gain ){
    float v = val - 0.5;
    return 1.0 / ( 1.0 + exp( -gain * v ) );
}

void main(void){
    
    vec2 texCoord = gl_FragCoord.xy / resolution;
    
    vec4 color = texture2D(tex,mod(texCoord,1.0));
    color *= 1.2 * ( 0.5 + 0.5 * sigmoid( sin( gl_FragCoord.y ), 2 ) );
    
    gl_FragColor = color;
    
}

/*
float round( float v ){
    if( v < 0.5 ){
        return 0.0;
    }else{
        return 1.0;
    }
}

float sigmoid( float val, float gain ){
    float v = val - 0.5;
    return 1.0 / ( 1.0 + exp( -gain * v ) );
}

void main(void){
    
    float t = 0.0;
    
    float y = floor( ( gl_FragCoord.y / resolution.y ) / 0.05 ) * 0.05;
    
    // x座標を 0.0 - 1.0 の値に変換する.
    float dx  = mod( gl_FragCoord.x / resolution.x + sin(y*20) * t, 1.0 );
    float rad = sin(radians(dx*360.0));
    
    float x = rad + sin(y * 3.14) * 100;
    
    float val = abs( floor( ( sin(x) * sin(x*2) * cos(exp(1)*x) + sin(x) ) / 0.25 ) * 0.25 );
    //float val = abs( floor( ( sin(x) * sin(x*2) * cos(exp(1)*x) + sin(x) ) / 0.0001 ) * 0.0001 );
    
    val = sigmoid( val, 100 );
    
    vec4 color = vec4( vec3(1.0), val );
    
    color.xyz *= vec3(0.5,0.9,1.0);
    
    if( y < 0.1 ){
        color *= 0.0;
    }
    
    gl_FragColor = color;
    
    
    
}
//*/