#include "VboSphere.h"
#include "cinder/Rand.h"

using namespace ci;

namespace hrfm{ namespace gl{
    
    //! public:
    
    void VboSphere::setup( ivec2 segments, bool curve, float noise ){
        
        ci::TriMesh * m = new ci::TriMesh();
        
        float x = 0.0f;
        float y = 0.0f;
        
        float stepX  = 1.0 / (float)segments.x;
        float stepY  = 1.0 / (float)segments.y;
        float radian = 3.141592653589;
        
        //*
        vector<vec3> vertices;
        for (int row = 0; row <= segments.y; row++){
            x = 0.0;
            for (int col = 0; col <= segments.x; col++){
                vertices.push_back(
                    vec3(
                        x-0.5 + stepX*randFloat(-noise,noise),
                        y-0.5 + stepY*randFloat(-noise,noise),
                        0.0 + randFloat(-noise,noise)*stepX
                    )
                );
                x += stepX;
            }
            y += stepY;
        }
        
        int numberVertices;
        
        for( int i=0; i<vertices.size()-segments.x-2; i++ ){
            
            if( (i % (segments.x+1) == segments.x) ){
                continue;
            }
            
            vec3 v0 = vertices.at(i);
            vec3 v1 = vertices.at(i+1);
            vec3 v2 = vertices.at(i+segments.x+1);
            vec3 v3 = vertices.at(i+segments.x+2);
            
            {
                vec3 v01 = v1-v0;
                vec3 v02 = v2-v0;
                vec3 norm = v01.cross(v02);
                
                m->appendVertex(v0);
                m->appendNormal(norm);
                m->appendTexCoord(vec2(v0.x+0.5,1.0-(v0.y+0.5)));
                m->appendVertex(v1);
                m->appendNormal(norm);
                m->appendTexCoord(vec2(v1.x+0.5,1.0-(v1.y+0.5)));
                m->appendVertex(v2);
                m->appendNormal(norm);
                m->appendTexCoord(vec2(v2.x+0.5,1.0-(v2.y+0.5)));
                numberVertices = m->getNumVertices();
                m->appendTriangle( numberVertices - 3, numberVertices - 2, numberVertices - 1 );
            }
            
            {
                vec3 v01 = v2-v3;
                vec3 v02 = v1-v3;
                vec3 norm = v01.cross(v02);

                m->appendVertex(v3);
                m->appendNormal(norm);
                m->appendTexCoord(vec2(v3.x+0.5,1.0-(v3.y+0.5)));
                m->appendVertex(v2);
                m->appendNormal(norm);
                m->appendTexCoord(vec2(v2.x+0.5,1.0-(v2.y+0.5)));
                m->appendVertex(v1);
                m->appendNormal(norm);
                m->appendTexCoord(vec2(v1.x+0.5,1.0-(v1.y+0.5)));
                numberVertices = m->getNumVertices();
                m->appendTriangle( numberVertices - 3, numberVertices - 2, numberVertices - 1 );
                
            }
            
        }
        
        /*/
        for (int i = 0; i < segments.x; i++){
            
            float x0 = cos(x*radian)*-0.5;
            float x1 = cos((x+stepX)*radian)*-0.5;
            
            float y0 = -0.5;
            float y1 = 0.5;
            
            float z0 = sin(x*radian);
            float z1 = sin((x+stepX)*radian);
            
            m->appendVertex(vec3(x0,-0.5,z0));
            m->appendTexCoord(vec2(x0+0.5,0));
            
            m->appendVertex(vec3(x1,-0.5,z1));
            m->appendTexCoord(vec2(x1+0.5, 0));
            
            m->appendVertex(vec3(x1,0.5,z1));
            m->appendTexCoord(vec2(x1+0.5, 1));
            
            m->appendVertex(vec3(x0,0.5,z0));
            m->appendTexCoord(vec2(x0+0.5, 1));
            
            int numberVertices = m->getNumVertices();
            
            m->appendTriangle( numberVertices - 4, numberVertices - 3, numberVertices - 2 );
            m->appendTriangle( numberVertices - 4, numberVertices - 2, numberVertices - 1 );
            
            x += stepX;
            
        }
        
        //*/
        
        this->mesh = new ci::gl::VboMesh( *m );
        
        ci::gl::Material * material = new ci::gl::Material();
        material->setAmbient( Color::black() );
        material->setDiffuse( Color(1.0,1.0,1.0) );
        material->setSpecular( Color(1.0,1.0,1.0) );
        material->setEmission( Color::black() );
        material->setShininess( 128.0f );
        
        this->setMaterial(material);
        
    }
    
}}