#version 400

uniform mat4 ciModel;
uniform mat4 ciModelView;
uniform mat4 ciModelViewProjection;
uniform mat3 ciNormalMatrix;

in  vec4 vColor;
in  vec3 vNormal;
in  vec4 vPosition;

out vec4 oColor;

void main(void){
    oColor = vColor;
}