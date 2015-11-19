#version 400

uniform mat4  ciModelViewProjection;

in  vec4 ciPosition;
in  vec4 ciColor;

out vec4 Color;

void main()
{
    gl_Position = ciModelViewProjection * ciPosition;
    Color = ciColor;
}