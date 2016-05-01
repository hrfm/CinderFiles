#version 400

layout (location = 0) in vec3 position;

uniform mat4 ciModel;
uniform mat4 ciModelView;
uniform mat4 ciModelViewProjection;
uniform mat3 ciNormalMatrix;

in  vec4 ciPosition;
in  vec3 ciNormal;
in  vec4 ciColor;

out vec4 vColor;
out vec3 vNormal;
out vec4 vPosition;

void main()
{
    vColor      = ciColor;
    vNormal     = ciNormal;
    vPosition   = ciPosition;
    gl_Position = vPosition;
}