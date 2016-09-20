#version 400

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

// -----------------------------------------------------
// --- uniforms

const float pi = 3.14159265;

// --- Data From Tessellation Shader.
uniform mat4  ciModel;
uniform mat4  ciModelView;
uniform mat4  ciModelViewProjection;
uniform mat3  ciNormalMatrix;

// -----------------------------------------------------
// --- in / out

in  vec4 vPosition[];
//in  vec3 vNormal[];
in  vec4 vColor[];

out vec4  gColor;
out vec3  gNormal;
out vec4  gPosition;

// ===================================================================================
// --- Functions.

mat4x4 identity(){
    return mat4x4(
                  vec4( 1.0, 0.0, 0.0, 0.0),
                  vec4( 0.0, 1.0, 0.0, 0.0),
                  vec4( 0.0, 0.0, 1.0, 0.0),
                  vec4( 0.0, 0.0, 0.0, 1.0)
                  );
}

// --- Color ---

float HueToRGB(float f1, float f2, float hue)
{
    if (hue < 0.0)
        hue += 1.0;
    else if (hue > 1.0)
        hue -= 1.0;
    float res;
    if ((6.0 * hue) < 1.0)
        res = f1 + (f2 - f1) * 6.0 * hue;
    else if ((2.0 * hue) < 1.0)
        res = f2;
    else if ((3.0 * hue) < 2.0)
        res = f1 + (f2 - f1) * ((2.0 / 3.0) - hue) * 6.0;
    else
        res = f1;
    return res;
}

vec3 HSLToRGB(vec3 hsl)
{
    vec3 rgb;
    
    if (hsl.y == 0.0)
        rgb = vec3(hsl.z, hsl.z, hsl.z); // Luminance
    else
    {
        float f2;
        
        if (hsl.z < 0.5)
            f2 = hsl.z * (1.0 + hsl.y);
        else
            f2 = (hsl.z + hsl.y) - (hsl.y * hsl.z);
        
        float f1 = 2.0 * hsl.z - f2;
        
        rgb.r = HueToRGB(f1, f2, hsl.x + (1.0/3.0));
        rgb.g = HueToRGB(f1, f2, hsl.x);
        rgb.b= HueToRGB(f1, f2, hsl.x - (1.0/3.0));
    }
    
    return rgb;
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

int getID( vec4 v ){
    //*
    float radV = 0;//atan( v.y, v.x ) * 64.0;
    float radH = atan( v.z, v.x ) * 64.0;
    return int( mod( (radV+radH)-mod(radV+radH,1.0), 16 ) );
    /*/
     float rand = rand(v.xy) * 64;
     return int( mod( (rand)-mod(rand,1.0), 16 ) );
     //*/
}

// -----------------------------------------------------
// --- emitter.

void emittVert( vec4 vert[3], vec4 mg, vec3 norm ){
    
    vec4 v, p;
    vec4 vPos[3];
    
    
    for(int i = 0; i < 3; ++i){
        vPos[i] = vec4( vec3( vert[i].xyz - mg.xyz ), 1.0 );
    }
    
    // ----------------------
    
    vec4 oVec[3];
    for(int i = 0; i < 3; ++i){
        int idx = getID(vert[i]);
        oVec[i] = vec4( (mg+vPos[i]).xyz, 1.0 );
    }
    
    gNormal = normalize(cross( oVec[2].xyz - oVec[0].xyz, oVec[1].xyz - oVec[0].xyz ));
    
    for(int i = 0; i < 3; ++i){
        vec4 color = vColor[i];
        float chroma = color.x * 0.3 + color.y * 0.59 + color.z * 0.11;
        gColor      = color;
        gPosition   = vec4( oVec[i].xyz + gNormal * chroma * 0.1, 1.0 );
        gl_Position = ciModelViewProjection * gPosition;
        EmitVertex();
    }
    
    EndPrimitive();
    
}

// ===================================================================================
// --- Main.

void main(){
    
    vec4 v[3];
    mat4x4 mtx = identity();
    
    v[0] = vPosition[0];
    v[1] = vPosition[1];
    v[2] = vPosition[2];
    
    vec4 mg   = ( v[0] + v[1] + v[2] ) / 3.0;
    vec3 norm = normalize( cross( v[2].xyz - v[0].xyz, v[1].xyz - v[0].xyz ) );
    emittVert( v, mg, norm );
    
}