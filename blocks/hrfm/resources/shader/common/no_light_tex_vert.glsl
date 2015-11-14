out vec4 vColor;
out vec3 normal;
uniform float time;

void main()
{
    gl_Position = ftransform();
    gl_FrontColor  = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    normal = gl_NormalMatrix * gl_Normal;
    vColor = gl_Color;
}