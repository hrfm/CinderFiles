#version 400

uniform mat4 ciModel;
uniform mat4 ciModelView;
uniform mat4 ciModelViewProjection;
uniform mat3 ciNormalMatrix;

uniform int       uUseTexture = 0;
uniform sampler2D tex;

in vec4 vColor;
in vec3 vNormal;
in vec4 vPosition;
in vec2 vTexCoord;

out vec4 oColor;

void main(void){
    
    if( uUseTexture == 1 ){
        oColor = texture( tex, vTexCoord );
    }else{
        oColor = vColor;
    }
    
}