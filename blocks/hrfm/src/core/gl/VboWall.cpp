#include "VboWall.h"

using namespace ci;

namespace hrfm{ namespace gl{
    
    //! public:
    
    void VboWall::setup(){
        
        ci::TriMesh * m = new ci::TriMesh();
        
        float x = 0.0f;
        int step = 100;
        float stepX = 1.0 / (float)step;
        
        float radian = 3.141592653589;
        
        for (int i = 0; i < step; i++){
            
            float x0 = cos(x*radian)*-0.5;
            float x1 = cos((x+stepX)*radian)*-0.5;
            
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
        
        this->mesh = new ci::gl::VboMesh( *m );
        
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
        if( _texture != NULL ){
            _texture->enableAndBind();
            if( _shader != NULL ){
                _shader->getGlslProgPtr()->uniform("tex", 0);
            }
        }
        ci::gl::draw( *this->mesh );
        if( _texture != NULL ){
            _texture->unbind();
        }
    };
    
}}