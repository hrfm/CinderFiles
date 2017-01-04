#include "VertBatchNode.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    //! public:
    
    void VertBatchNode::setup(){}
    
    void VertBatchNode::clear(){}
    
    void VertBatchNode::setBatchRef( ci::gl::VertBatchRef batchRef, ci::gl::GlslProgRef shader ){
        _batchRef = batchRef;
        _shader   = shader;
    }
    ci::gl::VertBatchRef VertBatchNode::getBatchRef(){
        return _batchRef;
    }
    
    void VertBatchNode::setEnableWireframe( bool flag ){
        _enableWireframe = flag;
    }
    
    ci::mat4 VertBatchNode::getRotation(){
        return this->rotation;
    }
    void VertBatchNode::setRotation( ci::mat4 rotate ){
        this->rotation = rotation;
    }
    
    void VertBatchNode::addTexture( ci::gl::TextureRef tex ){
        eraseTextureFromList(tex);
        _textures.push_back(tex);
    }
    void VertBatchNode::removeTexture( ci::gl::TextureRef tex ){
        eraseTextureFromList(tex);
    }
    
    void VertBatchNode::update(){
        if( visible() == false || colorA.a <= 0.0f ){
            return;
        }
        _update();
        _updateChildren();
    }
    
    void VertBatchNode::draw( ColorA * drawColor ){
        
        if( visible() == false || colorA.a <= 0.0f ){
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
            ci::gl::multModelMatrix(this->transform);
            ci::gl::translate( this->position() );
            //!!!!!! ci::gl::rotate( this->rotation );
            ci::gl::scale( this->scale );
            _draw();
            _drawChildren();
        }
        ci::gl::popModelMatrix();
        
        if( _enableWireframe  ) ci::gl::disableWireframe();
        ci::gl::disableAlphaBlending();
        
    }
    
    //! protected:
    
    void VertBatchNode::_update(){};
    
    void VertBatchNode::_draw(){
        if( _batchRef != NULL ){
            if( _shader != nullptr ){
                ci::gl::ScopedGlslProg glsl(_shader);
                vector<ci::gl::TextureRef>::iterator it, end;
                if( 0 < _textures.size() ){
                    int texIdx = 0;
                    for( it = _textures.begin(), end = _textures.end(); it!=end; it++ ){
                        (*it)->bind(texIdx);
                        _shader->uniform( "texture"+to_string(texIdx), texIdx );
                        texIdx++;
                    }
                }
                ci::gl::ScopedLineWidth lw(10.0);
                _batchRef->draw();
                if( 0 < _textures.size() ){
                    for( it = _textures.begin(), end = _textures.end(); it!=end; it++ ){
                        (*it)->unbind();
                    }
                }
            }else{
                _batchRef->draw();
            }
        }
    };
    
    inline bool VertBatchNode::eraseTextureFromList( ci::gl::TextureRef tex ){
        auto itr = std::remove_if(_textures.begin(),_textures.end(),[tex](ci::gl::TextureRef t)->bool{
            return t == tex;
        });
        if( itr == _textures.end() ){
            return false;
        }
        _textures.erase( itr, _textures.end() );
        return true;
    }
    
}}