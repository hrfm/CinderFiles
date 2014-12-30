#version 120

uniform sampler2D tex;
uniform vec2      sample_offset;
uniform float     attenuation;
uniform vec2      resolution;

void main(){
    
    vec4 sum = vec4( 0.0, 0.0, 0.0, 0.0 );
    
    // 各テクスチャ上の色を取得.
    vec2 pos = gl_FragCoord.xy / resolution;
    
    if( 0.0 < sample_offset.x ){
        
        vec2 offsetX = vec2( sample_offset.x, 0.0 );
        
        sum += texture2D( tex, pos + -10.0 * offsetX ).rgba * 0.009167927656011385;
        sum += texture2D( tex, pos +  -9.0 * offsetX ).rgba * 0.014053461291849008;
        sum += texture2D( tex, pos +  -8.0 * offsetX ).rgba * 0.020595286319257878;
        sum += texture2D( tex, pos +  -7.0 * offsetX ).rgba * 0.028855245532226279;
        sum += texture2D( tex, pos +  -6.0 * offsetX ).rgba * 0.038650411513543079;
        sum += texture2D( tex, pos +  -5.0 * offsetX ).rgba * 0.049494378859311142;
        sum += texture2D( tex, pos +  -4.0 * offsetX ).rgba * 0.060594058578763078;
        sum += texture2D( tex, pos +  -3.0 * offsetX ).rgba * 0.070921288047096992;
        sum += texture2D( tex, pos +  -2.0 * offsetX ).rgba * 0.079358891804948081;
        sum += texture2D( tex, pos +  -1.0 * offsetX ).rgba * 0.084895951965930902;
        sum += texture2D( tex, pos +   0.0 * offsetX ).rgba * 0.086826196862124602;
        sum += texture2D( tex, pos +  +1.0 * offsetX ).rgba * 0.084895951965930902;
        sum += texture2D( tex, pos +  +2.0 * offsetX ).rgba * 0.079358891804948081;
        sum += texture2D( tex, pos +  +3.0 * offsetX ).rgba * 0.070921288047096992;
        sum += texture2D( tex, pos +  +4.0 * offsetX ).rgba * 0.060594058578763078;
        sum += texture2D( tex, pos +  +5.0 * offsetX ).rgba * 0.049494378859311142;
        sum += texture2D( tex, pos +  +6.0 * offsetX ).rgba * 0.038650411513543079;
        sum += texture2D( tex, pos +  +7.0 * offsetX ).rgba * 0.028855245532226279;
        sum += texture2D( tex, pos +  +8.0 * offsetX ).rgba * 0.020595286319257878;
        sum += texture2D( tex, pos +  +9.0 * offsetX ).rgba * 0.014053461291849008;
        sum += texture2D( tex, pos + +10.0 * offsetX ).rgba * 0.009167927656011385;
        
    }
    
    if( 0.0 < sample_offset.y ){
        
        vec2 offsetY = vec2( 0.0, sample_offset.y );
        
        sum += texture2D( tex, pos + -10.0 * offsetY ).rgba * 0.009167927656011385;
        sum += texture2D( tex, pos +  -9.0 * offsetY ).rgba * 0.014053461291849008;
        sum += texture2D( tex, pos +  -8.0 * offsetY ).rgba * 0.020595286319257878;
        sum += texture2D( tex, pos +  -7.0 * offsetY ).rgba * 0.028855245532226279;
        sum += texture2D( tex, pos +  -6.0 * offsetY ).rgba * 0.038650411513543079;
        sum += texture2D( tex, pos +  -5.0 * offsetY ).rgba * 0.049494378859311142;
        sum += texture2D( tex, pos +  -4.0 * offsetY ).rgba * 0.060594058578763078;
        sum += texture2D( tex, pos +  -3.0 * offsetY ).rgba * 0.070921288047096992;
        sum += texture2D( tex, pos +  -2.0 * offsetY ).rgba * 0.079358891804948081;
        sum += texture2D( tex, pos +  -1.0 * offsetY ).rgba * 0.084895951965930902;
        sum += texture2D( tex, pos +   0.0 * offsetY ).rgba * 0.086826196862124602;
        sum += texture2D( tex, pos +  +1.0 * offsetY ).rgba * 0.084895951965930902;
        sum += texture2D( tex, pos +  +2.0 * offsetY ).rgba * 0.079358891804948081;
        sum += texture2D( tex, pos +  +3.0 * offsetY ).rgba * 0.070921288047096992;
        sum += texture2D( tex, pos +  +4.0 * offsetY ).rgba * 0.060594058578763078;
        sum += texture2D( tex, pos +  +5.0 * offsetY ).rgba * 0.049494378859311142;
        sum += texture2D( tex, pos +  +6.0 * offsetY ).rgba * 0.038650411513543079;
        sum += texture2D( tex, pos +  +7.0 * offsetY ).rgba * 0.028855245532226279;
        sum += texture2D( tex, pos +  +8.0 * offsetY ).rgba * 0.020595286319257878;
        sum += texture2D( tex, pos +  +9.0 * offsetY ).rgba * 0.014053461291849008;
        sum += texture2D( tex, pos + +10.0 * offsetY ).rgba * 0.009167927656011385;
        
    }
    
    if( sample_offset.x * sample_offset.y != 0.0 ){
        sum *= 0.5;
    }
    
    gl_FragColor.rgba = attenuation * sum;
    
}