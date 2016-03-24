#version 400

layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;

uniform mat4  ciModel;
uniform mat4  ciModelView;
uniform mat4  ciModelViewProjection;
uniform mat3  ciNormalMatrix;

uniform float uTime;

uniform float uNumTexture      = 0.0;

uniform vec3  uBias            = vec3(0.0);

uniform vec3  uParticleScale   = vec3(1.0);

uniform float uNormalMoveScale = 1.0;

uniform vec3  uRadialCenter    = vec3(0.0);
uniform float uRadialMoveScale = 0.0;

in  vec4 vPosition[];
in  vec3 vNormal[];
in  vec4 vColor[];

out float gId;
out vec4  gColor;
out vec3  gNormal;
out vec2  gTexCoord;
out vec4  gPosition;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void emittVert( vec3 vert[3], int idx, vec3 mg ){
    
    vec3 v, p;
    vec4 vPos[3], vPos2[3];
    
    for(int i = 0; i < 3; ++i){
        v = vert[i].xyz - mg;
        p = vert[i].xyz;//0.2*v;
        vPos2[i] = vec4(v,1.0);
        vPos[i]  = vec4(p,1.0);
    }
    
    float speed = rand( mg.xy ) * 3.0;
    vec3  bias  = uBias * uTime;
    float rad   = uTime * speed * 4.0;
    
    mat4x4 rotY = mat4x4(
        vec4( cos(rad), 0.0, -sin(rad), 0.0),
        vec4(      0.0, 1.0,       0.0, 0.0),
        vec4( sin(rad), 0.0,  cos(rad), 0.0),
        vec4(      0.0, 0.0,       0.0, 1.0)
    );
    
    mat4x4 rotX = mat4x4(
        vec4( 1.0,       0.0,      0.0, 0.0),
        vec4( 0.0,  cos(rad), sin(rad), 0.0),
        vec4( 0.0, -sin(rad), cos(rad), 0.0),
        vec4( 0.0,       0.0,      0.0, 1.0)
    );
    
    mat4x4 rotZ = mat4x4(
        vec4(  cos(rad), sin(rad), 0.0, 0.0),
        vec4( -sin(rad), cos(rad), 0.0, 0.0),
        vec4(       0.0,      0.0, 1.0, 0.0),
        vec4(       0.0,      0.0, 0.0, 1.0)
    );
    
    mat4x4 scale = mat4x4(
        vec4(  uParticleScale.x,              0.0,              0.0, 0.0),
        vec4(               0.0, uParticleScale.y,              0.0, 0.0),
        vec4(               0.0,              0.0, uParticleScale.z, 0.0),
        vec4(               0.0,              0.0,              0.0, 1.0)
    );
    
    gId     = mod( ( mg.x * mg.z ) / 0.01, 1.0 );
    gNormal = normalize(cross( vPos[2].xyz - vPos[0].xyz, vPos[1].xyz - vPos[0].xyz ));
    
    vec3 radialVec = normalize( mg - uRadialCenter );
    
    for(int i = 0; i < 3; ++i){
        
        gColor    = vColor[i];
        
        if( uTime == 0.0 ){
            gPosition = vPos[i];
        }else{
            gPosition = (rotY*rotX*rotZ*scale) * vPos2[i]
                        + (vPos[i]-vPos2[i])
                        + vec4( ( (-gNormal*uNormalMoveScale) + (radialVec*uRadialMoveScale) + bias ) * (1.0/max(1.0,uNormalMoveScale)+uTime*speed), 1.0 )
            ;
        }
        
        if( idx == 0 ){
            if( i== 0 ){
                gTexCoord = vec2(0,0);
            }else if( i == 1 ){
                gTexCoord = vec2(1,0);
            }else{
                gTexCoord = vec2(0,1);
            }
        }else{
            if( i== 0 ){
                gTexCoord = vec2(1,1);
            }else if( i == 1 ){
                gTexCoord = vec2(0,1);
            }else{
                gTexCoord = vec2(1,0);
            }
        }
        
        gl_Position = ciModelViewProjection * gPosition;
        EmitVertex();
        
    }
    
    EndPrimitive();
    
}

void main(){
    
    vec3 v0, v1, v2, v3, v[3], mg;
    v0 = vPosition[0].xyz;
    v1 = vPosition[1].xyz;
    v2 = vPosition[2].xyz;
    v3 = v2+(v1-v0);
    
    if( uNumTexture == 0 ){
        mg = ( v0 + v1 + v2 ) / 3.0;
    }else{
        mg = ( v0 + v1 + v2 + v3 ) / 4.0;
    }
    
    if( uTime == 0.0 || uNumTexture == 0 ){
        
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
        emittVert( v, 0, mg );
        
    }else{
        
        if( gl_PrimitiveIDIn%2 == 0 ){
            return;
        }
        
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
        emittVert( v, 0, mg );
        
        v[0] = v3;
        v[1] = v2;
        v[2] = v1;
        emittVert( v, 1, mg );
        
    }
    
}