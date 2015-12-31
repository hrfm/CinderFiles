#include "BatchNode.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    //! public:
    
    void BatchNode::setup(){}
    
    void BatchNode::clear(){}
    
    void BatchNode::setBatchRef( ci::gl::BatchRef batchRef ){
        _batchRef = batchRef;
    }
    ci::gl::BatchRef BatchNode::getBatchRef(){
        return _batchRef;
    }
    
    void BatchNode::setEnableWireframe( bool flag ){
        _enableWireframe = flag;
    }
    
    ci::mat4 BatchNode::getRotation(){
        return this->rotation;
    }
    void BatchNode::setRotation( ci::mat4 rotate ){
        this->rotation = rotation;
    }
    
    void BatchNode::addTexture( ci::gl::TextureRef tex ){
        eraseTextureFromList(tex);
        _textures.push_back(tex);
    }
    void BatchNode::removeTexture( ci::gl::TextureRef tex ){
        eraseTextureFromList(tex);
    }
    
    void BatchNode::update(){
        if( visible == false || colorA.a <= 0.0f ){
            return;
        }
        _update();
        _updateChildren();
    }
    
    void BatchNode::draw( ColorA * drawColor ){
        
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
        
        ci::gl::enableAlphaBlending();
        ci::gl::color( c );
        if( _enableWireframe  ) ci::gl::enableWireframe();
        
        ci::gl::enableDepthRead();
        ci::gl::enableDepthWrite();
        
        ci::gl::pushModelMatrix();
        {
            ci::gl::translate( getPosition() );
            //!!!!!! ci::gl::rotate( this->rotation );
            ci::gl::scale( this->scale );
            _draw();
            _drawChildren(&c);
        }
        ci::gl::popModelMatrix();
        
        /*
        
        vector<ci::gl::TextureRef>::iterator it, end;
        
        ci::gl::pushModelMatrix();
        {
            
            if( 0 < _textures.size() ){
                int texIdx = 3;
                for( it = _textures.begin(), end = _textures.end(); it!=end; it++ ){
                    (*it)->bind(texIdx);
                    _batchRef->getGlslProg()->uniform( "texture"+toString(texIdx), texIdx );
                    texIdx++;
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
         
        }
        ci::gl::popModelMatrix();
        
        //*/
        
        if( _enableWireframe  ) ci::gl::disableWireframe();
        ci::gl::disableAlphaBlending();
        
    }
    
    //! protected:
    
    void BatchNode::_update(){};
    
    void BatchNode::_draw(){
        if( _batchRef != NULL ){
            _batchRef->draw();
        }
    };
    void BatchNode::_drawForLights(){}
    void BatchNode::_drawChildren( ColorA * drawColor ){
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
    
    inline bool BatchNode::eraseTextureFromList( ci::gl::TextureRef tex ){
        auto itr = std::remove_if(_textures.begin(),_textures.end(),[tex](ci::gl::TextureRef t)->bool{
            return t == tex;
        });
        if( itr == _textures.end() ){
            return false;
        }
        _textures.erase( itr, _textures.end() );
        return true;
    }
    
    void BatchNode::_appendVertex( ci::TriMeshRef m, vec3 v0, vec3 v1, vec3 v2 ){
        
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
    
}}