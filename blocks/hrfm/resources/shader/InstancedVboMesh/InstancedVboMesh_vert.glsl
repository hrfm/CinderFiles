#version 400

layout (location = 0) in vec3 position;

uniform mat4 ciModel;
uniform mat4 ciModelView;
uniform mat4 ciModelViewProjection;
uniform mat3 ciNormalMatrix;

in vec4   ciPosition;
in vec3   ciNormal;
in vec4   ciColor;
in vec2   ciTexCoord0;
in mat4x4 uInstanceTransform;

out vec4 vColor;
out vec3 vNormal;
out vec4 vPosition;
out vec2 vTexCoord;

void main()
{
    vColor      = ciColor;
    vNormal     = ciNormal;
    vPosition   = uInstanceTransform * ciPosition;
    vTexCoord   = ciTexCoord0;
    gl_Position = ciModelViewProjection * vPosition;
}