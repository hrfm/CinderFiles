#version 120

uniform sampler2D tex;
uniform vec2      sample_offset;
uniform float     attenuation;
uniform vec2 resolution;

void main()
{
    
    vec4 sum = vec4( 0.0, 0.0, 0.0, 0.0 );
    
    // 各テクスチャ上の色を取得.
    vec2 pos = gl_FragCoord.xy / resolution;
    
    sum += texture2D( tex, pos + -10.0 * sample_offset ).rgba * 0.009167927656011385;
    sum += texture2D( tex, pos +  -9.0 * sample_offset ).rgba * 0.014053461291849008;
    sum += texture2D( tex, pos +  -8.0 * sample_offset ).rgba * 0.020595286319257878;
    sum += texture2D( tex, pos +  -7.0 * sample_offset ).rgba * 0.028855245532226279;
    sum += texture2D( tex, pos +  -6.0 * sample_offset ).rgba * 0.038650411513543079;
    sum += texture2D( tex, pos +  -5.0 * sample_offset ).rgba * 0.049494378859311142;
    sum += texture2D( tex, pos +  -4.0 * sample_offset ).rgba * 0.060594058578763078;
    sum += texture2D( tex, pos +  -3.0 * sample_offset ).rgba * 0.070921288047096992;
    sum += texture2D( tex, pos +  -2.0 * sample_offset ).rgba * 0.079358891804948081;
    sum += texture2D( tex, pos +  -1.0 * sample_offset ).rgba * 0.084895951965930902;
    sum += texture2D( tex, pos +   0.0 * sample_offset ).rgba * 0.086826196862124602;
    sum += texture2D( tex, pos +  +1.0 * sample_offset ).rgba * 0.084895951965930902;
    sum += texture2D( tex, pos +  +2.0 * sample_offset ).rgba * 0.079358891804948081;
    sum += texture2D( tex, pos +  +3.0 * sample_offset ).rgba * 0.070921288047096992;
    sum += texture2D( tex, pos +  +4.0 * sample_offset ).rgba * 0.060594058578763078;
    sum += texture2D( tex, pos +  +5.0 * sample_offset ).rgba * 0.049494378859311142;
    sum += texture2D( tex, pos +  +6.0 * sample_offset ).rgba * 0.038650411513543079;
    sum += texture2D( tex, pos +  +7.0 * sample_offset ).rgba * 0.028855245532226279;
    sum += texture2D( tex, pos +  +8.0 * sample_offset ).rgba * 0.020595286319257878;
    sum += texture2D( tex, pos +  +9.0 * sample_offset ).rgba * 0.014053461291849008;
    sum += texture2D( tex, pos + +10.0 * sample_offset ).rgba * 0.009167927656011385;
    
    gl_FragColor.rgba = attenuation * sum;
//    gl_FragColor.a = 1.0;
    
}