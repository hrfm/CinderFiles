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
    
    void VboNode::setShader( hrfm::gl::ShaderBase * shader ){
        _shader = shader;
    }
    
    /*!!!!!!!
    void VboNode::setMaterial( ci::gl::Material * material ){
        _material = material;
    }
    ci::gl::Material * VboNode::getMaterial(){
        return _material;
    }
    //*/
    
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
            ci::gl::translate( getPosition() );
            //!!!!!! ci::gl::rotate( this->rotation );
            ci::gl::scale( this->scale );
            
            if( _enableWireframe  ) ci::gl::enableWireframe();
            
            //!!!!!! if( _material != NULL ) _material->apply();
            
            if( _shader != NULL ){
                ci::gl::ScopedGlslProg scpGlsl( _shader->getGlslProg() );
                _shader->prepare();
                _shader->getGlslProg()->uniform( "alpha", colorA.a );
                if( 0 < _textures.size() ){
                    int texIdx = 0;
                    for( it = _textures.begin(), end = _textures.end(); it!=end; it++ ){
                        (*it)->bind(texIdx);
                        _shader->getGlslProg()->uniform( "texture"+toString(texIdx), texIdx );
                    }
                }
                _draw();
                if( 0 < _textures.size() ){
                    for( it = _textures.begin(), end = _textures.end(); it!=end; it++ ){
                        (*it)->unbind();
                    }
                }
                _drawChildren(&c);
                _shader->clear();
            }else{
                if( 0 < _textures.size() ){
                    for( it = _textures.begin(), end = _textures.end(); it!=end; it++ ){
                        (*it)->bind();
                    }
                }
                _draw();
                if( 0 < _textures.size() ){
                    for( it = _textures.begin(), end = _textures.end(); it!=end; it++ ){
                        (*it)->unbind();
                    }
                }
                _drawChildren(&c);
            }
            
            if( _enableWireframe  ) ci::gl::disableWireframe();
            
        }
        ci::gl::popMatrices();
        
        ci::gl::disableAlphaBlending();
    }
    
    //! protected:
    
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
    
    void VboNode::_update(){};
    
    void VboNode::_draw(){
        ci::gl::draw(this->mesh);
    };
    
    //! protected
    
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
    
}}