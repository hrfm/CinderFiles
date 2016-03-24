#version 400

layout (location = 0) in vec3 position;

uniform mat4  ciModelViewProjection;

in  vec4 ciPosition;
in  vec4 ciColor;

out vec4 vColor;
out vec4 vPosition;
out vec4 vMVPPosition;

void main()
{
    vColor       = ciColor;
    vPosition    = ciPosition;
    vMVPPosition = ciModelViewProjection * ciPosition;
    gl_Position  = ciPosition;
}