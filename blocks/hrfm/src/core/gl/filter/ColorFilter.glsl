#version 400

uniform sampler2D tex;
uniform float     time;
uniform vec2      resolution;

uniform sampler2D colorTex;
uniform int       useColorTex;

uniform vec3      color;
uniform int       mode;
uniform int       inverse;

out vec4 oColor;

void main(void){
    
    vec2 texCoord = gl_FragCoord.xy / resolution;
    
    vec4 texColor = texture( tex, texCoord );
    
    if( color.r == 0 && color.g == 0 && color.b == 0 ){
        float chroma = texColor.x * 0.3 + texColor.y * 0.59 + texColor.z * 0.11;
        texColor = vec4( vec3(chroma), 1.0 );
    }else if(mode == 0){
        texColor.r *= color.x;
        texColor.g *= color.y;
        texColor.b *= color.z;
    }else{
        texColor.r += color.x;
        if( 1.0 < texColor.r ){
            texColor.r = 1.0;
        }else if( texColor.r < 0.0 ){
            texColor.r = 0.0;
        }
        texColor.g += color.y;
        if( 1.0 < texColor.g ){
            texColor.g = 1.0;
        }else if( texColor.g < 0.0 ){
            texColor.g = 0.0;
        }
        texColor.b += color.z;
        if( 1.0 < texColor.b ){
            texColor.b = 1.0;
        }else if( texColor.b < 0.0 ){
            texColor.b = 0.0;
        }
    }
    
    if( useColorTex == 1 ){
        texColor.rgb *= texture( colorTex, texCoord ).rgb;
    }
    
    if( inverse == 1 ){
        texColor.r = 1.0 - texColor.x;
        texColor.g = 1.0 - texColor.y;
        texColor.b = 1.0 - texColor.z;
    }
    
    oColor = texColor;
    
}