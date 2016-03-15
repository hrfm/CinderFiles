#version 410

in vec4 ciPosition;
in vec3 ciNormal;
in vec4 ciColor;

uniform mat4 ciModelMatrix;
uniform mat4 ciModelView;
uniform mat3 ciNormalMatrix;
uniform mat4 ciModelViewProjection;

// ShadowMatrix converts from modeling coordinates to shadow map coordinates.
uniform mat4 uShadowMatrix;


out vec4	vColor;         // Vertex Color.
out vec4	vPosition;      // EyeSpace Position.
out vec3	vNormal;        //
out vec4	vModelPosition; // WorldSpace Position.
out vec3	vModelNormal;   // WorldSpace Normal
out vec2	vTexCoord0;     //
// Coordinate to be used for shadow map lookup
out vec4	vShadowCoord;   //

/* Bias matrix alters the clip coordinates so that x & y
 * lie between 0.0 and 1.0 for texture sampling. */
const mat4 biasMat  = mat4(	0.5, 0.0, 0.0, 0.0,
							0.0, 0.5, 0.0, 0.0,
							0.0, 0.0, 0.5, 0.0,
							0.5, 0.5, 0.5, 1.0 );
void main() {
    
    // モデルの色を取得する.
	vColor			= ciColor;
    // カメラ座標系に変換する.
	vPosition		= ciModelView * ciPosition;
    // ワールド座標系に変換する.
	vModelPosition	= ciModelMatrix * ciPosition;
    // 法線ベクトルをワールド座標系に変換する?
	vModelNormal	= (ciModelMatrix * vec4(ciNormal, 0.0)).xyz;
    // これなんだろう？
	vNormal			= normalize( ciNormalMatrix * ciNormal );
	// これなんだろう？
	vShadowCoord	= (biasMat * uShadowMatrix * ciModelMatrix) * ciPosition;
    
    // モデルのWindow座標系.
	gl_Position		= ciModelViewProjection * ciPosition;
    
}