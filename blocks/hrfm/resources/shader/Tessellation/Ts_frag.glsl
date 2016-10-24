#version 400

const mat4 biasMat = mat4(
                          0.5, 0.0, 0.0, 0.0,
                          0.0, 0.5, 0.0, 0.0,
                          0.0, 0.0, 0.5, 0.0,
                          0.5, 0.5, 0.5, 1.0
                          );

uniform mat4 ciModel;
uniform mat4 ciModelView;
uniform mat4 ciModelViewProjection;
uniform mat3 ciNormalMatrix;

uniform sampler2D tex;

in  vec4 gColor;
in  vec3 gNormal;
in  vec4 gPosition;
out vec4 oColor;

void main(void){
    vec4 p      = ciModelViewProjection * gPosition;
    vec3  invLight = normalize( inverse(ciModel) * vec4(vec3(1.0,0.0,1.0)-p.xyz, 0.0)).xyz;
    float diffuse  = clamp(dot(gNormal, invLight), 0.1, 1.0);
    oColor         = vec4(1.0) * vec4(vec3(diffuse), 1.0);
}