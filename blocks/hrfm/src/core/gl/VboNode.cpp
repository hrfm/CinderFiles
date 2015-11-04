#include "VboNode.h"

using namespace ci;

namespace hrfm{ namespace gl{
    
    //! public:
    
    void VboNode::setup(){}
    
    void VboNode::clear(){}
    
    vec3 VboNode::getPosition(){
        return this->position;
    }
    void VboNode::setPosition( vec3 position ){
        this->position = position;
    }
    
    vec3 VboNode::getScale(){
        return this->scale;
    }
    void VboNode::setScale( float scale ){
        this->scale = vec3( scale, scale, scale );
    }
    void VboNode::setScale( vec3 scale ){
        this->scale = scale;
    }
    
    ci::Matrix44f VboNode::getRotation(){
        return this->rotation;
    }
    void VboNode::setRotation( ci::Matrix44f rotate ){
        this->rotation = rotation;
    }
    
    vec3 VboNode::getAbsolutePosition(){
        if( hasParent() ){
            return position + _parent->getAbsolutePosition();
        }else{
            return position;
        }
    }
    
    void VboNode::setEnableWireframe( bool flag ){
        _enableWireframe = flag;
    }
    
    void VboNode::setShader( hrfm::gl::ShaderBase * shader ){
        _shader = shader;
    }
    
    void VboNode::setMaterial( ci::gl::Material * material ){
        _material = material;
    }
    ci::gl::Material * VboNode::getMaterial(){
        return _material;
    }
    
    void VboNode::addTexture( ci::gl::Texture * tex ){
        eraseTextureFromList(tex);
        _textures.push_back(tex);
    }
    void VboNode::removeTexture( ci::gl::Texture * tex ){
        eraseTextureFromList(tex);
    }
    
    int VboNode::numChildren(){
        return children.size();
    }
    
    VboNode * VboNode::addChild( VboNode * child ){
        
        eraseFromChildren(child);
        
        if( hasStage() ){
            child->_setStage(_stage);
        }
        child->_setParent(this);
        
        children.push_back(child);
        
        return child;
        
    }
    
    VboNode * VboNode::removeChild( VboNode * child ){
        
        if( eraseFromChildren(child) ){
            child->_unsetStage();
            child->_unsetParent();
        }
        
        return child;
        
    }
    
    VboNode * VboNode::addChildAt( VboNode * child, int index ){
        if( numChildren() < index || index < 0 ){
            throw;
        }else if( numChildren() == index ){
            return addChild(child);
        }else{
            std::vector<VboNode*>::iterator itr = children.begin() + index;
            if( *itr != child ){
                eraseFromChildren(child);
                children.insert(itr, child);
            }
            return child;
        }
    }
    
    VboNode * VboNode::removeChildAt( int index ){
        if( numChildren() <= index || index < 0 ){
            throw;
        }else{
            std::vector<VboNode*>::iterator itr = children.begin() + index;
            children.erase(itr);
            return *itr;
        }
    }
    
    VboNode * VboNode::removeOwn(){
        if( hasParent() ){
            getParent()->removeChild(this);
        }
        return this;
    }
    
    bool VboNode::hasChildOf( VboNode * child ){
        auto itr = std::remove_if(children.begin(),children.end(),[child](VboNode* d)->bool{
            return d == child;
        });
        if( itr == children.end() ){
            return false;
        }
        return true;
    }
    
    void VboNode::update( ci::CameraPersp * camera ){
        if( visible == false || colorA.a <= 0.0f ){
            return;
        }
        _update( camera );
        _updateChildren( camera );
    }
    
    void VboNode::draw( ci::CameraPersp * camera, ColorA * drawColor ){
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
        
        vector<ci::gl::Texture*>::iterator it, end;
        
        ci::gl::pushMatrices();
        {
            ci::gl::translate( this->position );
            ci::gl::rotate( this->rotation );
            ci::gl::scale( this->scale );
            
            if( _enableWireframe  ) ci::gl::enableWireframe();
            
            if( _material != NULL ) _material->apply();
            
            if( _shader != NULL ){
                _shader->begin();
                _shader->getGlslProgPtr()->uniform( "alpha", colorA.a );
                if( 0 < _textures.size() ){
                    int texIdx = 0;
                    for( it = _textures.begin(), end = _textures.end(); it!=end; it++ ){
                        (*it)->bind(texIdx);
                        _shader->getGlslProgPtr()->uniform( "texture"+toString(texIdx), texIdx );
                    }
                }
            }else{
                if( 0 < _textures.size() ){
                    for( it = _textures.begin(), end = _textures.end(); it!=end; it++ ){
                        (*it)->bind();
                    }
                }
            }
            
            _draw( camera );
            
            if( 0 < _textures.size() ){
                for( it = _textures.begin(), end = _textures.end(); it!=end; it++ ){
                    (*it)->unbind();
                }
            }
            
            _drawChildren( camera, &c );
            
            if( _shader != NULL ) _shader->end();
            
            if( _enableWireframe  ) ci::gl::disableWireframe();
            
        }
        ci::gl::popMatrices();
        
        ci::gl::disableAlphaBlending();
    }
    
    bool VboNode::hasParent(){
        return _parent != nullptr && _parent != NULL;
    }
    
    VboNode * VboNode::getParent(){
        return _parent;
    }
    
    bool VboNode::hasStage(){
        return _stage != nullptr && _stage != NULL;
    }
    
    VboStage * VboNode::getStage(){
        return _stage;
    }
    
    //! protected:
    
    void VboNode::_appendVertex( ci::TriMesh & m, vec3 v0, vec3 v1, vec3 v2 ){
        
        vec3 v01 = v1-v0;
        vec3 v02 = v2-v0;
        vec3 norm = v01.cross(v02);
        
        m.appendVertex(v0);
        m.appendNormal(norm);
        m.appendVertex(v1);
        m.appendNormal(norm);
        m.appendVertex(v2);
        m.appendNormal(norm);
        
        int numberVertices = m.getNumVertices();
        m.appendTriangle( numberVertices - 3, numberVertices - 2, numberVertices - 1 );
        
    }
    
    void VboNode::_update( ci::CameraPersp * camera ){};
    
    void VboNode::_draw( ci::CameraPersp * camera ){
        ci::gl::draw( *this->mesh );
    };
    
    inline bool VboNode::eraseFromChildren( VboNode * child ){
        auto itr = std::remove_if(children.begin(),children.end(),[child](VboNode* d)->bool{
            return d == child;
        });
        if( itr == children.end() ){
            return false;
        }
        children.erase( itr, children.end() );
        return true;
    }
    
    void VboNode::_updateChildren( ci::CameraPersp * camera ){
        if( numChildren() == 0 ) return;
        std::vector<VboNode*>::iterator it, end;
        for( it = children.begin(), end = children.end(); it!=end; it++ ){
            if( *it!=nullptr ){
                (*it)->update( camera );
            }
        }
    }
    
    void VboNode::_drawChildren( ci::CameraPersp * camera, ColorA * drawColor ){
        if( numChildren() == 0 ) return;
        std::vector<VboNode*>::iterator it, end;
        for( it = children.begin(), end = children.end(); it!=end; it++ ){
            if( *it!=nullptr ){
                (*it)->draw( camera, drawColor );
            }
        }
    }
    
    //! protected
    
    inline bool VboNode::eraseTextureFromList( ci::gl::Texture * tex ){
        auto itr = std::remove_if(_textures.begin(),_textures.end(),[tex](ci::gl::Texture* t)->bool{
            return t == tex;
        });
        if( itr == _textures.end() ){
            return false;
        }
        _textures.erase( itr, _textures.end() );
        return true;
    }
    
    void VboNode::_setParent( VboNode * node ){
        if( !hasParent() ){
            _parent = node;
            dispatchEvent( new hrfm::events::Event(hrfm::events::Event::ADDED) );
        }else{
            _parent = node;
        }
    }
    
    void VboNode::_unsetParent(){
        if( hasStage() ){
            _stage = NULL;
            dispatchEvent( new hrfm::events::Event(hrfm::events::Event::REMOVED) );
        }else{
            _stage = NULL;
        }
    }
    
    void VboNode::_setStage( VboStage * stage ){
        
        if( !hasStage() ){
            _stage = stage;
            dispatchEvent( new hrfm::events::Event(hrfm::events::Event::ADDED_TO_STAGE) );
        }else{
            _stage = stage;
        }
        
        if( numChildren() == 0 ) return;
        
        std::vector<VboNode*>::iterator it, end;
        for( it = children.begin(), end = children.end(); it!=end; it++ ){
            if( *it!=nullptr ){
                (*it)->_setStage( stage );
            }
        }
        
    }
    
    void VboNode::_unsetStage(){
        
        if( hasStage() ){
            _stage = NULL;
            dispatchEvent( new hrfm::events::Event(hrfm::events::Event::REMOVED_FROM_STAGE) );
        }else{
            _stage = NULL;
        }
        
        if( numChildren() == 0 ) return;
        
        std::vector<VboNode*>::iterator it, end;
        for( it = children.begin(), end = children.end(); it!=end; it++ ){
            if( *it!=nullptr ){
                (*it)->_unsetStage();
            }
        }
        
    }
    
}}