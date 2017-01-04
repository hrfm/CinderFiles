#include "DisplayNode.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    //! public:
    
    void DisplayNode::setup(){}
    
    void DisplayNode::clear(){}
    
    Rectf DisplayNode::getBounds(){
        return this->_bounds;
    }
    
    Rectf DisplayNode::getDrawBounds(){
        cout << "getDrawBounds : " << this->_drawBounds << endl;
        return this->_drawBounds;
    }
    
    ci::vec3 DisplayNode::getAbsolutePosition(){
        return this->getGlobalPosition();
    }
    
    ci::vec3 DisplayNode::getGlobalPosition(){
        if( this->hasParent() ){
            return this->position() + this->getParent()->getGlobalPosition();
        }else{
            return this->position();
        }
    }
    
    void DisplayNode::enableAdditiveBlending(){
        this->_enableAdditiveBlending = true;
    }
    void DisplayNode::disableAdditiveBlending(){
        this->_enableAdditiveBlending = false;
    }
    
    bool DisplayNode::isResized(){
        return this->_resized;
    }
    
    int DisplayNode::numChildren(){
        return this->children.size();
    }
    
    bool DisplayNode::hasChildOf( DisplayNode * child ){
        auto itr = std::remove_if(this->children.begin(),this->children.end(),[child](DisplayNode* d)->bool{
            return d == child;
        });
        if( itr == this->children.end() ){
            return false;
        }
        return true;
    }
    
    bool DisplayNode::hasParent(){
        return this->_parent != nullptr && this->_parent != NULL;
    }
    DisplayNode * DisplayNode::getParent(){
        return this->_parent;
    }
    
    bool DisplayNode::isRootNode(){
        return !this->hasParent();
    }
    DisplayNode * DisplayNode::getRootNode(){
        if( this->isRootNode() ){
            return this;
        }else{
            return this->getParent()->getRootNode();
        }
    }
    
    bool DisplayNode::hasStage(){
        return this->_stage != nullptr && this->_stage != NULL;
    }
    IStage * DisplayNode::getStage(){
        return this->_stage;
    }
    
    DisplayNode * DisplayNode::addChild( DisplayNode * child ){
        this->eraseFromChildren(child);
        if( this->hasStage() ){ child->_setStage(this->_stage); }
        child->_setParent(this);
        this->children.push_back(child);
        return child;
    }
    
    DisplayNode * DisplayNode::addChildAt( DisplayNode * child, int index ){
        if( this->numChildren() < index || index < 0 ){
            throw;
        }else if( this->numChildren() == index ){
            return this->addChild(child);
        }
        std::vector<DisplayNode*>::iterator itr = this->children.begin() + index;
        if( *itr != child ){
            this->eraseFromChildren(child);
            this->children.insert(itr, child);
        }
        return child;
    }
    
    DisplayNode * DisplayNode::removeChild( DisplayNode * child ){
        if( this->eraseFromChildren(child) ){
            child->_unsetStage();
            child->_unsetParent();
        }
        return child;
    }
    
    DisplayNode * DisplayNode::removeChildAt( int index ){
        if( this->numChildren() <= index || index < 0 ){ throw; }
        std::vector<DisplayNode*>::iterator itr = this->children.begin() + index;
        this->children.erase(itr);
        return *itr;
    }
    
    DisplayNode * DisplayNode::removeOwn(){
        if( this->hasParent() ){ this->getParent()->removeChild(this); }
        return this;
    }
    
    void DisplayNode::setUpdateFrequency( unsigned int freq ){
        this->_updateFrequency = this->_updateCount = freq;
    }
    
    void DisplayNode::update(){
        if( !this->visible() ){
            return;
        }
        uint32_t elapsedFrame = ci::app::getElapsedFrames();
        if( elapsedFrame - this->_beforeUpdateFrame < this->_updateFrequency ){
            return;
        }
        this->_beforeUpdateFrame = elapsedFrame;
        // TODO 毎回呼ぶのはちょっと回数が多そう
        if( this->_beforeSize.x != this->width() || this->_beforeSize.y != this->height() ){
            this->_resized = true;
            this->_beforeSize = this->_size;
            this->dispatchEvent( new hrfm::events::Event( hrfm::events::Event::RESIZE ) );
        }
        this->_update();
        this->_updateChildren();
    }
    
    void DisplayNode::draw(){
        if( !this->visible() || this->colorA.a == 0.0f ){
            return;
        }
        ci::gl::pushModelMatrix();
        {
            ci::gl::ScopedColor color( this->colorA );
            //ci::gl::multModelMatrix(this->transform);
            //ci::gl::translate( this->position() );
            //ci::gl::rotate( this->rotation.w, this->rotation.x, this->rotation.y, this->rotation.z );
            //ci::gl::scale( this->scale );
            if( this->_enableAdditiveBlending ){
                ci::gl::enableAlphaBlending();
                _draw();
                _drawChildren();
                ci::gl::disableAlphaBlending();
            }else{
                _draw();
                _drawChildren();
            }
            /*
            // この機能は Stage3Dに委譲する
            if( camera != NULL ){
                
                ci::gl::setMatrices( *camera );
                ci::gl::ScopedViewport vpt( getSize() );
                
                ci::gl::multModelMatrix(this->transform);
                ci::gl::translate( getPosition() );
                ci::gl::rotate( rotate );
                
                ci::gl::ScopedDepth dpt( true );
                ci::gl::enableDepthWrite();
                ci::gl::enableDepthRead();
                
                if( _enableAdditiveBlending == true ){
                    ci::gl::enableAdditiveBlending();
                }else if( c.a < 1.0f ){
                    ci::gl::enableAlphaBlendingPremult();
                }
                ci::gl::color( c );
                _draw();
                ci::gl::disableAlphaBlending();
                ci::gl::color(1.0,1.0,1.0);
                
                _drawChildren( &c );
                
                ci::gl::disableDepthRead();
                ci::gl::disableDepthWrite();
                
            }else{
                
                ci::gl::multModelMatrix(this->transform);
                ci::gl::translate( getPosition() );
                ci::gl::rotate( rotate );
                
                ci::gl::color( c );
                if( _enableAdditiveBlending == true ){
                    ci::gl::enableAdditiveBlending();
                }else if( c.a < 1.0f ){
                    ci::gl::enableAlphaBlendingPremult();
                }
                _draw();
                ci::gl::disableAlphaBlending();
                ci::gl::color(1.0,1.0,1.0);
                
                _drawChildren( &c );
                
            }
             */
            
        }
        ci::gl::popModelMatrix();
        
        this->_resized = false;
        
    }
    
    //! protected:
    
    inline bool DisplayNode::eraseFromChildren( DisplayNode * child ){
        auto itr = std::remove_if(children.begin(),children.end(),[child](DisplayNode* d)->bool{
            return d == child;
        });
        if( itr == children.end() ){
            return false;
        }
        children.erase( itr, children.end() );
        return true;
    }
    
    void DisplayNode::_update(){}
    void DisplayNode::_updateChildren(){
        if( numChildren() == 0 ) return;
        std::vector<DisplayNode*>::iterator it, end;
        for( it = children.begin(), end = children.end(); it!=end; it++ ){
            if( *it!=nullptr ){
                (*it)->update();
            }
        }
    }
    
    void DisplayNode::_draw(){};
    void DisplayNode::_drawChildren(){
        if( numChildren() == 0 ) return;
        std::vector<DisplayNode*>::iterator it, end;
        for( it = children.begin(), end = children.end(); it!=end; it++ ){
            if( *it!=nullptr ){
                (*it)->draw();
            }
        }
    }
    
    void DisplayNode::_setParent( DisplayNode * node ){
        if( !hasParent() ){
            _parent = node;
            dispatchEvent( new hrfm::events::Event(hrfm::events::Event::ADDED) );
        }else{
            _parent = node;
        }
    }
    
    void DisplayNode::_unsetParent(){
        if( hasStage() ){
            _stage = NULL;
            dispatchEvent( new hrfm::events::Event(hrfm::events::Event::REMOVED) );
        }else{
            _stage = NULL;
        }
    }
    
    void DisplayNode::_setStage( IStage * stage ){
        if( !hasStage() ){
            _stage = stage;
            dispatchEvent( new hrfm::events::Event(hrfm::events::Event::ADDED_TO_STAGE) );
        }else{
            _stage = stage;
        }
        if( numChildren() == 0 ) return;
        std::vector<DisplayNode*>::iterator it, end;
        for( it = children.begin(), end = children.end(); it!=end; it++ ){
            if( *it!=nullptr ){
                (*it)->_setStage( stage );
            }
        }
    }
    
    void DisplayNode::_unsetStage(){
        if( hasStage() ){
            _stage = NULL;
            dispatchEvent( new hrfm::events::Event(hrfm::events::Event::REMOVED_FROM_STAGE) );
        }else{
            _stage = NULL;
        }
        if( numChildren() == 0 ) return;
        std::vector<DisplayNode*>::iterator it, end;
        for( it = children.begin(), end = children.end(); it!=end; it++ ){
            if( *it!=nullptr ){
                (*it)->_unsetStage();
            }
        }
    }
    
}}