#version 400

layout (location = 0) in vec3 position;

uniform mat4  ciModelViewProjection;

in  vec4 ciPosition;
in  vec4 ciColor;

out vec4 vColor;
out vec4 vPosition;

void main()
{
    vColor = ciColor;
    vPosition = ciModelViewProjection * ciPosition;
    gl_Position = vPosition;
}