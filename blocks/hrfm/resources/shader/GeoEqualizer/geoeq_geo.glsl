#version 400

layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;

uniform mat4  ciModel;
uniform mat4  ciModelView;
uniform mat4  ciModelViewProjection;
uniform mat3  ciNormalMatrix;

uniform float uWave[64];
uniform float uFFT[16];
uniform float uVolume;

uniform float uTime;
uniform vec3  uRadialCenter    = vec3(0.0);
uniform float uRadialMoveScale = 0.0;

in  vec4 vPosition[];
in  vec3 vNormal[];
in  vec4 vColor[];

out vec4  gColor;
out vec3  gNormal;
out vec4  gPosition;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void emittVert( vec3 vert[3], vec3 mg ){
    
    vec3 v, p;
    vec3 vPos[3];
    
    for(int i = 0; i < 3; ++i){
        vPos[i] = vert[i].xyz - mg;
    }
    
    vec3 tmpVert[3];
    vec3 radialVec = normalize( mg - uRadialCenter );
    
    for(int i = 0; i < 3; ++i){
        
        vec3  vert = mg + vPos[i];
        float radV = atan( vert.y, vert.x ) * 64.0;
        float radH = atan( vert.z, vert.x ) * 64.0;
        int idx    = int( mod( (radV+radH)-mod(radV+radH,1.0), 16 ) );
        
        float val = uFFT[idx] * 0.1;
        if( val < 0.1 ){
            val = 0.1;
        }
        val = 1.0 + val * uVolume;
        
        tmpVert[i] = (mg+vPos[i]) * val;
        
    }
    
    gNormal = normalize(cross( tmpVert[2] - tmpVert[0], tmpVert[1] - tmpVert[0] ));
    
    for(int i = 0; i < 3; ++i){
        gColor      = vColor[i];
        //gPosition   = vec4( tmpVert[i] + gNormal * uVolume, 1.0 );
        gPosition   = vec4( tmpVert[i], 1.0 );
        gl_Position = ciModelViewProjection * gPosition;
        EmitVertex();
    }
    
    EndPrimitive();
    
}

void main(){
    
    vec3 v0, v1, v2, v[3], mg;
    v0 = vPosition[0].xyz;
    v1 = vPosition[1].xyz;
    v2 = vPosition[2].xyz;
    mg = ( v0 + v1 + v2 ) / 3.0;
    
    v[0] = v0;
    v[1] = v1;
    v[2] = v2;
    emittVert( v, mg );
    
}