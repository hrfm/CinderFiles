#version 400

uniform mat4 ciModel;
uniform mat4 ciModelView;
uniform mat4 ciModelViewProjection;
uniform mat3 ciNormalMatrix;

uniform sampler2D texture0;
uniform float     shadowDistanceFrom = 0.0;
uniform float     shadowDistance = 30.0;

in  vec4 vColor;
in  vec3 vNormal;
in  vec4 vPosition;
in  vec4 vMVPPosition;
out vec4 oColor;

void main(void){
    
    vec2 texCoord = vPosition.xy + vec2(0.5,0.5);
    //texCoord = vec2(texCoord.y,texCoord.x);
    
    vec4 color = texture( texture0, texCoord );
    color = vec4(1.0);
    
    color *= max(0.6,-vPosition.z);
    
    float len = length(vMVPPosition);
    if( len < shadowDistanceFrom ){
        oColor = color;
    }else{
        oColor = color * vec4( vec3( 1.0 - ((len-shadowDistanceFrom)/shadowDistance) ), 1.0 );
    }
    
    vec3 lightPos       = vec3( -1000.0, -100.0, 0.0 );
    vec3 normal         = ciNormalMatrix * vNormal;//ciNormalMatrix * vNormal;
    vec3 surfaceToLight = lightPos - vPosition.xyz;
    
    float brightness = dot( normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
    brightness = clamp(brightness, 0, 1);
    
    oColor = vec4( oColor.xyz * (1.0-brightness), oColor.w );
    
}