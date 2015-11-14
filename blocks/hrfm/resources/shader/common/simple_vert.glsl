#version 150

uniform mat4  ciModelViewProjection;
uniform float time;

in  vec4 ciPosition;
in  vec4 ciColor;
out vec4 Color;
out vec2 TexCoord0;

void main()
{
    gl_Position = ciModelViewProjection * ciPosition;
    Color = ciColor;
}