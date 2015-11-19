#version 400

uniform sampler2D tex;
uniform vec2      sample_offset;
uniform float     attenuation;
uniform vec2      resolution;

out vec4 oColor;

vec4 sumfnc( vec4 base, vec2 pos, vec2 offset ){
    
    //*
    base += texture( tex, pos + -5.0 * offset ).rgba * 0.009167927656011385;
    base += texture( tex, pos + -4.5 * offset ).rgba * 0.014053461291849008;
    base += texture( tex, pos + -4.0 * offset ).rgba * 0.020595286319257878;
    base += texture( tex, pos + -3.5 * offset ).rgba * 0.028855245532226279;
    base += texture( tex, pos + -3.0 * offset ).rgba * 0.038650411513543079;
    base += texture( tex, pos + -2.5 * offset ).rgba * 0.049494378859311142;
    base += texture( tex, pos + -2.0 * offset ).rgba * 0.060594058578763078;
    base += texture( tex, pos + -1.5 * offset ).rgba * 0.070921288047096992;
    //*/
    base += texture( tex, pos + -1.0 * offset ).rgba * 0.079358891804948081;
    base += texture( tex, pos + -0.5 * offset ).rgba * 0.084895951965930902;
    base += texture( tex, pos +  0.0 * offset ).rgba * 0.086826196862124602;
    base += texture( tex, pos +  0.5 * offset ).rgba * 0.084895951965930902;
    base += texture( tex, pos +  1.0 * offset ).rgba * 0.079358891804948081;
    //*
    base += texture( tex, pos +  1.5 * offset ).rgba * 0.070921288047096992;
    base += texture( tex, pos +  2.0 * offset ).rgba * 0.060594058578763078;
    base += texture( tex, pos +  2.5 * offset ).rgba * 0.049494378859311142;
    base += texture( tex, pos +  3.0 * offset ).rgba * 0.038650411513543079;
    base += texture( tex, pos +  3.5 * offset ).rgba * 0.028855245532226279;
    base += texture( tex, pos +  4.0 * offset ).rgba * 0.020595286319257878;
    base += texture( tex, pos +  4.5 * offset ).rgba * 0.014053461291849008;
    base += texture( tex, pos +  5.0 * offset ).rgba * 0.009167927656011385;
    //*/
    
    return base;
    
}

void main(){
    
    vec2 pos = gl_FragCoord.xy / resolution;
    pos.y = 1.0 - pos.y;

    /*
    
    vec4 sum = texture2D( tex, pos );
    sum *= 0.36;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2(-1.0,  1.0)) / resolution) * 0.04;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2( 0.0,  1.0)) / resolution) * 0.04;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2( 1.0,  1.0)) / resolution) * 0.04;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2(-1.0,  0.0)) / resolution) * 0.04;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2( 1.0,  0.0)) / resolution) * 0.04;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2(-1.0, -1.0)) / resolution) * 0.04;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2( 0.0, -1.0)) / resolution) * 0.04;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2( 1.0, -1.0)) / resolution) * 0.04;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2(-2.0,  2.0)) / resolution) * 0.02;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2(-1.0,  2.0)) / resolution) * 0.02;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2( 0.0,  2.0)) / resolution) * 0.02;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2( 1.0,  2.0)) / resolution) * 0.02;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2( 2.0,  2.0)) / resolution) * 0.02;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2(-2.0,  1.0)) / resolution) * 0.02;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2( 2.0,  1.0)) / resolution) * 0.02;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2(-2.0,  0.0)) / resolution) * 0.02;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2( 2.0,  0.0)) / resolution) * 0.02;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2(-2.0, -1.0)) / resolution) * 0.02;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2( 2.0, -1.0)) / resolution) * 0.02;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2(-2.0, -2.0)) / resolution) * 0.02;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2(-1.0, -2.0)) / resolution) * 0.02;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2( 0.0, -2.0)) / resolution) * 0.02;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2( 1.0, -2.0)) / resolution) * 0.02;
    sum += texture2D(tex, (gl_FragCoord.xy + vec2( 2.0, -2.0)) / resolution) * 0.02;
    
    /*/
    
    vec4 sum = vec4( 0.0, 0.0, 0.0, 0.0 );
    if( 0.0 < sample_offset.x ){
        sum = sumfnc( sum, pos, vec2( sample_offset.x, 0.0 ) );
    }
    if( 0.0 < sample_offset.y ){
        sum = sumfnc( sum, pos, vec2( 0.0, sample_offset.y ) );
    }
    if( sample_offset.x * sample_offset.y != 0.0 ){
        sum *= 0.5;
    }
    
    //*/
    
    oColor = sum * attenuation;
    
}