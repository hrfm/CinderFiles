#version 400

layout( quads, equal_spacing, ccw ) in;

uniform sampler2D tex;

in vec3 EvaluationPointWorld[];
in vec4 TcsColor[];

out vec4 vPosition;
out vec4 vColor;

// gl_TessCoord is location within the patch
// (barycentric for triangles, UV for quads)
void main ()
{
    /*
     vec3 p0 = gl_TessCoord.x * EvaluationPointWorld[0]; // x is one corner
     vec3 p1 = gl_TessCoord.y * EvaluationPointWorld[1]; // y is the 2nd corner
     vec3 p2 = gl_TessCoord.z * EvaluationPointWorld[2]; // z is the 3rd corner (ignored when using quads)
     vPosition  = vec4( p0 + p1 + p2, 1.0 );
     /*/
    vec3 p0 = mix( EvaluationPointWorld[0], EvaluationPointWorld[1], gl_TessCoord.x );
    vec3 p1 = mix( EvaluationPointWorld[2], EvaluationPointWorld[3], gl_TessCoord.x );
    vec3 pos = mix( p0, p1, gl_TessCoord.y );
    vPosition = vec4( pos, 1.0 );
    //*/
    
    vec4 c0 = gl_TessCoord.x * TcsColor[0];
    vec4 c1 = gl_TessCoord.y * TcsColor[1];
    vec4 c2 = gl_TessCoord.z * TcsColor[2];
    
    //if( mod( vPosition.y, 0.1 ) < 0.005 ){
        vColor = texture(tex,vPosition.xy + vec2(0.5));
    //}else{
    //    vColor = vec4(0);
    //}
    
}