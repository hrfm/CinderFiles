#version 400

uniform mat4  ciModelViewProjection;

in  vec4 ciPosition;
in  vec4 ciColor;

out vec4 vColor;
out vec4 vPosition;

void main()
{
    gl_Position = ciModelViewProjection * ciPosition;
    vColor    = ciColor;
    vPosition = gl_Position;
}