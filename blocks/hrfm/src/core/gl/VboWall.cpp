#include "VboWall.h"

#include "cinder/Rand.h"

using namespace ci;

namespace hrfm{ namespace gl{
    
    //! public:
    
    void VboWall::setup( Vec2i segments, bool lines, float noise ){
        
        ci::TriMesh * m = new ci::TriMesh();
        
        float x = 0.0f;
        float y = 0.0f;
        
        float stepX  = 1.0 / (float)segments.x;
        float stepY  = 1.0 / (float)segments.y;
        float radian = 3.141592653589;
        
        //*
        vector<Vec3f> vertices;
        for (int row = 0; row <= segments.y; row++){
            x = 0.0;
            for (int col = 0; col <= segments.x; col++){
                if( 0.0 != noise ){
                    vertices.push_back(
                        Vec3f(
                            x-0.5 + stepX*randFloat(-noise,noise),
                            y-0.5 + stepY*randFloat(-noise,noise),
                            0.0 + randFloat(-noise,noise)*stepX
                        )
                    );
                }else{
                    vertices.push_back(Vec3f(x-0.5,y-0.5,0.0));
                }
                x += stepX;
            }
            y += stepY;
        }
        
        int numberVertices;
        
        for( int i=0; i<vertices.size()-segments.x-2; i++ ){
            
            if( (i % (segments.x+1) == segments.x) ){
                continue;
            }
            
            Vec3f v0 = vertices.at(i);
            Vec3f v1 = vertices.at(i+1);
            Vec3f v2 = vertices.at(i+segments.x+1);
            Vec3f v3 = vertices.at(i+segments.x+2);
            
            if(lines){
                
                {
                    Vec3f v01 = v1-v0;
                    Vec3f v02 = v2-v0;
                    Vec3f norm = v01.cross(v02);
                    
                    m->appendVertex(v0);
                    m->appendNormal(norm);
                    m->appendTexCoord(Vec2f(v0.x+0.5,1.0-(v0.y+0.5)));
                    m->appendVertex(v1);
                    m->appendNormal(norm);
                    m->appendTexCoord(Vec2f(v1.x+0.5,1.0-(v1.y+0.5)));
                    m->appendVertex(v0);
                    m->appendNormal(norm);
                    m->appendTexCoord(Vec2f(v0.x+0.5,1.0-(v0.y+0.5)));
                    numberVertices = m->getNumVertices();
                    m->appendTriangle( numberVertices - 3, numberVertices - 2, numberVertices - 1 );
                }
                
            }else{
                
                {
                    Vec3f v01 = v1-v0;
                    Vec3f v02 = v2-v0;
                    Vec3f norm = v01.cross(v02);
                    
                    m->appendVertex(v0);
                    m->appendNormal(norm);
                    m->appendTexCoord(Vec2f(v0.x+0.5,1.0-(v0.y+0.5)));
                    m->appendVertex(v1);
                    m->appendNormal(norm);
                    m->appendTexCoord(Vec2f(v1.x+0.5,1.0-(v1.y+0.5)));
                    m->appendVertex(v2);
                    m->appendNormal(norm);
                    m->appendTexCoord(Vec2f(v2.x+0.5,1.0-(v2.y+0.5)));
                    numberVertices = m->getNumVertices();
                    m->appendTriangle( numberVertices - 3, numberVertices - 2, numberVertices - 1 );
                }
                {
                    Vec3f v01 = v2-v3;
                    Vec3f v02 = v1-v3;
                    Vec3f norm = v01.cross(v02);
                    
                    m->appendVertex(v3);
                    m->appendNormal(norm);
                    m->appendTexCoord(Vec2f(v3.x+0.5,1.0-(v3.y+0.5)));
                    m->appendVertex(v2);
                    m->appendNormal(norm);
                    m->appendTexCoord(Vec2f(v2.x+0.5,1.0-(v2.y+0.5)));
                    m->appendVertex(v1);
                    m->appendNormal(norm);
                    m->appendTexCoord(Vec2f(v1.x+0.5,1.0-(v1.y+0.5)));
                    numberVertices = m->getNumVertices();
                    m->appendTriangle( numberVertices - 3, numberVertices - 2, numberVertices - 1 );
                    
                }
                
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
            
            m->appendVertex(Vec3f(x0,-0.5,z0));
            m->appendTexCoord(Vec2f(x0+0.5,0));
            
            m->appendVertex(Vec3f(x1,-0.5,z1));
            m->appendTexCoord(Vec2f(x1+0.5, 0));
            
            m->appendVertex(Vec3f(x1,0.5,z1));
            m->appendTexCoord(Vec2f(x1+0.5, 1));
            
            m->appendVertex(Vec3f(x0,0.5,z0));
            m->appendTexCoord(Vec2f(x0+0.5, 1));
            
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
    
    void VboWall::clear(){}
    
    void VboWall::setTexture( ci::gl::Texture * tex ){
        if( _texture != NULL ){
            _texture->unbind();
        }
        _texture = tex;
    }
    
    //! protected:
    
    void VboWall::_update( ci::CameraPersp * camera ){};
    
    void VboWall::_draw( ci::CameraPersp * camera ){
        
        hrfm::io::SiKORGMIDIInterface * KORG = &hrfm::io::SiKORGMIDIInterface::getInstance();
        
        if( _texture != NULL ){
            _texture->enableAndBind();
            if( _shader != NULL ){
                _shader->getGlslProgPtr()->uniform("tex", 0);
                hrfm::io::SiAudioInput * audio = &hrfm::io::SiAudioInput::getInstance();
                _shader->getGlslProgPtr()->uniform( "strength", KORG->nanoKontrolFader[11] );
                _shader->getGlslProgPtr()->uniform( "volume", audio->getVolume() );
                _shader->getGlslProgPtr()->uniform( "wave", audio->getChannelAt(0), 128 );
                _shader->getGlslProgPtr()->uniform( "basePos", Vec3f(0.0,0.8,0.0) );
                _shader->getGlslProgPtr()->uniform( "alpha", colorA.a );
            }
        }
        ci::gl::draw( *this->mesh );
        if( _texture != NULL ){
            _texture->unbind();
        }
    };
    
}}