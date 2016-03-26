#version 400

uniform mat4 ciModel;
uniform mat4 ciModelView;
uniform mat4 ciModelViewProjection;
uniform mat3 ciNormalMatrix;

uniform float uTime;
uniform float uFadeOut = 3.0;
uniform float uNumTexture;
uniform sampler2D uTex[16];

uniform float shadowDistanceFrom = 5.0;
uniform float shadowDistance = 35.0;

in  float gId;
in  vec4  gColor;
in  vec3  gNormal;
in  vec4  gPosition;
in  vec2  gTexCoord;

out vec4 oColor;

void main(void){
    
    vec4 color = gColor;
    if( 0.0 < uTime && 0 < uNumTexture ){
        float step = 1.0/uNumTexture;
        for( int i=0; i<uNumTexture; i++ ){
            if( gId < step*float(i+1) ){
                color = texture( uTex[i], gTexCoord );
                break;
            }
        }
    }
    oColor = color;
    
    // ------- 距離で暗くする.
    
    //*
    vec4  p   = ciModelViewProjection * gPosition;
    float len = length(p);
    if( shadowDistanceFrom < len ){
        oColor *= vec4( vec3( 1.0 - ((len-shadowDistanceFrom)/shadowDistance) ), 1.0 );
    }
    //*/
    
    if( uFadeOut < uTime ){
        oColor *= ( 1.0 - ((uTime-uFadeOut)/1.0) );
    }
    
    // -------
    
    /*
    vec3 lightPos       = vec3( 1000.0, 1000.0, 0.0 );
    vec3 normal         = ciNormalMatrix * gNormal;//ciNormalMatrix * vNormal;
    vec3 surfaceToLight = lightPos - p.xyz;
    
    float brightness = dot( normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
    brightness = clamp(brightness, 0, 1);
    
    oColor = vec4( oColor.xyz * (1.0-brightness), oColor.w );
    //*/
    
}