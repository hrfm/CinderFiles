#version 400

uniform mat4 ciModel;
uniform mat4 ciModelView;
uniform mat4 ciModelViewProjection;
uniform mat3 ciNormalMatrix;

uniform vec3 uLightPos = vec3( -100.0,100.0, 0.0 );

in  vec4  gColor;
in  vec3  gNormal;
in  vec4  gPosition;

out vec4 oColor;

void main(void){
    
    vec4 color = gColor;
    
    oColor = color;
    
    //return;
    
    vec4 p              = ciModelViewProjection * gPosition;
    vec3 normal         = ciNormalMatrix * gNormal;//ciNormalMatrix * vNormal;
    
    /*
    
    vec3 surfaceToLight = uLightPos - p.xyz;
    float lightDistance = length(surfaceToLight);
    surfaceToLight      = normalize( surfaceToLight );
    
    float diffuse = dot(surfaceToLight, normal);
    
    float brightness = dot( normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
    brightness = clamp(brightness, 0, 1);
    
    oColor = vec4( oColor.xyz * (1.0-brightness) * diffuse, oColor.w );
    
    /*/
    
    vec3 surfaceToLight = uLightPos - p.xyz;
    float brightness = dot( normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
    brightness = clamp(brightness, 0, 1);
    oColor = vec4( oColor.xyz * (1.0-brightness), oColor.w );
    
    //*/
    
}