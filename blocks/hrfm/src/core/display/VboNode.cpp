#include "VboNode.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    //! public:
    
    void VboNode::setup(){}
    
    void VboNode::clear(){}
    
    
    ci::mat4 VboNode::getRotation(){
        return this->rotation;
    }
    void VboNode::setRotation( ci::mat4 rotate ){
        this->rotation = rotation;
    }
    
    void VboNode::setEnableWireframe( bool flag ){
        _enableWireframe = flag;
    }
    
    void VboNode::setVboMeshRef( ci::gl::VboMeshRef vboMeshRef ){
        _vboMeshRef = vboMeshRef;
        if( _vboMeshRef == NULL ){
            _batchRef = NULL;
        }else if( _glslProgRef != NULL ){
            _batchRef = ci::gl::Batch::create( _vboMeshRef, _glslProgRef );
        }
    }
    ci::gl::VboMeshRef VboNode::getVboMeshRef(){
        return _vboMeshRef;
    }
    
    void VboNode::setShader( ci::gl::GlslProgRef shader ){
        _glslProgRef = shader;
        if( _glslProgRef == NULL ){
            _batchRef = NULL;
        }else if( _vboMeshRef != NULL ){
            _batchRef = ci::gl::Batch::create( _vboMeshRef, _glslProgRef );
        }
    }
    ci::gl::GlslProgRef VboNode::getShader(){
        return _glslProgRef;
    }
    
    void VboNode::addTexture( ci::gl::TextureRef tex ){
        eraseTextureFromList(tex);
        _textures.push_back(tex);
    }
    void VboNode::removeTexture( ci::gl::TextureRef tex ){
        eraseTextureFromList(tex);
    }
    
    void VboNode::update(){
        if( visible == false || colorA.a <= 0.0f ){
            return;
        }
        _update();
        _updateChildren();
    }
    
    void VboNode::draw( ColorA * drawColor ){
        if( visible == false || colorA.a <= 0.0f ){
            return;
        }
        ColorA c = ColorA(colorA.r,colorA.g,colorA.b,colorA.a);
        if( drawColor != NULL ){
            c.r *= drawColor->r;
            c.g *= drawColor->g;
            c.b *= drawColor->b;
            c.a *= drawColor->a;
        }
        //if( c.a < 1.0f ){
            ci::gl::enableAlphaBlending();
        //}else{
            //ci::gl::disableAlphaBlending();
        //}
        ci::gl::color( c );
        
        vector<ci::gl::TextureRef>::iterator it, end;
        
        ci::gl::pushMatrices();
        {
            
            if( _enableWireframe  ) ci::gl::enableWireframe();
            //!!!!!! if( _material != NULL ) _material->apply();
            
            if( 0 < _textures.size() ){
                int texIdx = 0;
                for( it = _textures.begin(), end = _textures.end(); it!=end; it++ ){
                    (*it)->bind(texIdx);
                    if( _glslProgRef != NULL ) _glslProgRef->uniform( "texture"+toString(texIdx), texIdx );
                }
            }
            
            ci::gl::translate( getPosition() );
            //!!!!!! ci::gl::rotate( this->rotation );
            ci::gl::scale( this->scale );
            _draw();
            
            if( 0 < _textures.size() ){
                for( it = _textures.begin(), end = _textures.end(); it!=end; it++ ){
                    (*it)->unbind();
                }
            }
            _drawChildren(&c);
            
            if( _enableWireframe  ) ci::gl::disableWireframe();
            
        }
        ci::gl::popMatrices();
        
        ci::gl::disableAlphaBlending();
    }
    
    //! protected:
    
    void VboNode::_update(){};
    
    void VboNode::_draw(){
        if( _batchRef != NULL ){
            _batchRef->draw();
        }else if( _vboMeshRef != NULL ){
            ci::gl::draw(this->_vboMeshRef);
        }
    };
    void VboNode::_drawChildren( ColorA * drawColor ){
        DisplayNode::_drawChildren( drawColor );
        /*
        if( numChildren() == 0 ) return;
        std::vector<DisplayNode*>::iterator it, end;
        for( it = children.begin(), end = children.end(); it!=end; it++ ){
            if( *it!=nullptr ){
                (*it)->draw( drawColor );
            }
        }
        //*/
    }
    
    inline bool VboNode::eraseTextureFromList( ci::gl::TextureRef tex ){
        auto itr = std::remove_if(_textures.begin(),_textures.end(),[tex](ci::gl::TextureRef t)->bool{
            return t == tex;
        });
        if( itr == _textures.end() ){
            return false;
        }
        _textures.erase( itr, _textures.end() );
        return true;
    }
    
    void VboNode::_appendVertex( ci::TriMeshRef m, vec3 v0, vec3 v1, vec3 v2 ){
        
        vec3 v01 = v1-v0;
        vec3 v02 = v2-v0;
        vec3 norm = ci::cross(v01,v02);
        
        m->appendPosition(v0);
        m->appendNormal(norm);
        m->appendPosition(v1);
        m->appendNormal(norm);
        m->appendPosition(v2);
        m->appendNormal(norm);
        
        int numberVertices = m->getNumVertices();
        m->appendTriangle( numberVertices - 3, numberVertices - 2, numberVertices - 1 );
        
    }
    
    /*!!!!!!!
     void VboNode::setMaterial( ci::gl::Material * material ){
     _material = material;
     }
     ci::gl::Material * VboNode::getMaterial(){
     return _material;
     }
     //*/
    
}}