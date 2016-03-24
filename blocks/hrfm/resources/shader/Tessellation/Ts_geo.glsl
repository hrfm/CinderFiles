#version 400

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 ciModelViewProjection;
uniform sampler2D texture0;

in vec4 TesPosition[];
out vec4 vPosition;
out vec3 vNormal;
out vec4 vMVPPosition;

void main()
{
    
    // 三角形ポリゴンの重心
    vec3 mg = vec3(0.0);
    for(int i = 0; i < 3; ++i){
        mg += TesPosition[i].xyz;
    }
    mg /= 3.0;
    
    vec3 v, p;
    vec4 vPos[3];
    
    for(int i = 0; i < 3; ++i){
        
        v = mg-TesPosition[i].xyz;
        p = TesPosition[i].xyz;//0.2*v;
        
        vec2 texCoord = p.xy + vec2(0.5,0.5);
        //texCoord = vec2(texCoord.y,texCoord.x);
        
        vec4 color = texture( texture0, texCoord );
        float chroma = color.x * 0.3 + color.y * 0.59 + color.z * 0.11; // 明るさを計算
        p.z = - (chroma*chroma) * 4.0;
        
        vPos[i] = vec4(p,1.0);
        
    }
    
    vNormal = normalize(cross( vPos[2].xyz - vPos[0].xyz, vPos[1].xyz - vPos[0].xyz ));
    
    for(int i = 0; i < 3; ++i){
        vPosition = vPos[i];
        vMVPPosition = ciModelViewProjection * vPosition;
        gl_Position = vMVPPosition;
        EmitVertex();
    }
    
    EndPrimitive();
    
}