#version 400

// number of control points in patch
layout (vertices = 4) out;

uniform float uTessLevelInner = 100.0;
uniform float uTessLevelOuter = 100.0;

in vec4 vPosition[];
in vec4 vNormal[];
in vec4 vColor[];

out vec3 EvaluationPointWorld[];
out vec4 TcsColor[];

void main ()
{
    EvaluationPointWorld[gl_InvocationID] = vPosition[gl_InvocationID].xyz;
    TcsColor[gl_InvocationID] = vColor[gl_InvocationID];
    // Calculate the tessellation levels
    gl_TessLevelInner[0] = uTessLevelInner; // number of nested primitives to generate
    gl_TessLevelInner[1] = uTessLevelInner; // number of nested primitives to generate
    gl_TessLevelOuter[0] = uTessLevelOuter; // times to subdivide first side
    gl_TessLevelOuter[1] = uTessLevelOuter; // times to subdivide second side
    gl_TessLevelOuter[2] = uTessLevelOuter; // times to subdivide third side
    gl_TessLevelOuter[3] = uTessLevelOuter; // times to subdivide third side
}