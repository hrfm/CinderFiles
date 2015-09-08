varying vec4 vColor;
uniform float time;

void main()
{
    gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = ftransform();
    vColor = gl_Color;
}