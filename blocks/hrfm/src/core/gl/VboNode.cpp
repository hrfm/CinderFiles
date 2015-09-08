#include "VboNode.h"

using namespace ci;

namespace hrfm{ namespace gl{
    
    //! public:
    
    void VboNode::setup(){}
    
    void VboNode::clear(){}
    
    Vec3f VboNode::getScale(){
        return this->scale;
    }
    
    void VboNode::setScale( Vec3f scale ){
        this->scale = scale;
    }
    
    ci::Matrix44f VboNode::getRotation(){
        return this->rotation;
    }
    void VboNode::setRotation( ci::Matrix44f rotate ){
        this->rotation = rotation;
    }
    
    Vec3f VboNode::getAbsolutePosition(){
        Vec3f pos( x, y, z );
        if( hasParent() ){
            return pos + _parent->getAbsolutePosition();
        }else{
            return pos;
        }
    }
    
    void VboNode::setShader( hrfm::gl::ShaderBase * shader ){
        _shader = shader;
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
        if( c.a < 1.0f ){
            ci::gl::enableAlphaBlending();
        }else{
            ci::gl::disableAlphaBlending();
        }
        ci::gl::pushMatrices();
        {
            ci::gl::translate( x, y );
            ci::gl::color( c );
            _draw( camera );
            _drawChildren( camera, &c );
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
    
    void VboNode::_update( ci::CameraPersp * camera ){};
    
    void VboNode::_draw( ci::CameraPersp * camera ){
        ci::gl::pushMatrices();
        {
            ci::gl::translate( Vec3f( x, y, z ) );
            ci::gl::rotate( this->rotation );
            ci::gl::scale( this->scale );
            //ci::gl::enableWireframe();
            if( _shader != NULL ){
                _shader->begin();
                
                GLfloat lightpos[] = { 0.0f, 0.0f, -1000.0f, 1.0f }; /* 位置　　　 */
                GLfloat lightcol[] = { 1.0f, 1.0f, 1.0f, 1.0f }; /* 直接光強度 */
                GLfloat lightamb[] = { 0.1f, 0.1f, 0.1f, 1.0f }; /* 環境光強度 */
                
                glEnable(GL_LIGHTING);
                glEnable(GL_LIGHT0);
                glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
                glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
                glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
                glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

                
                ci::gl::draw( *this->mesh );
                _shader->end();
            }else{
                ci::gl::draw( *this->mesh );
            }
            //ci::gl::disableWireframe();
        }
        ci::gl::pushMatrices();
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