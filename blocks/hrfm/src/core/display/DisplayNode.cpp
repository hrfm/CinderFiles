#include "DisplayNode.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    //! public:
    
    void DisplayNode::setup(){}
    
    void DisplayNode::clear(){}
    
    void DisplayNode::enableAdditiveBlending( bool flag ){
        _enableAdditiveBlending = flag;
    }
    
    vec3 DisplayNode::getPosition(){
        return vec3(x,y,z);
    }
    void DisplayNode::setPosition( float x, float y ){
        _updatePosition( x, y, this->z );
    }
    void DisplayNode::setPosition( float x, float y, float z ){
        _updatePosition( x, y, z );
    }
    void DisplayNode::setPosition( vec2 position ){
        _updatePosition( position.x, position.y, this->z );
    }
    void DisplayNode::setPosition( vec3 position ){
        _updatePosition( position.x, position.y, position.z );
    }
    
    ivec2 DisplayNode::getSize(){
        return ivec2( width, height );
    }
    void DisplayNode::setSize( int w, int h ){
        width  = w;
        height = h;
        if( _beforeWidth != width || _beforeHeight != height ){
            dispatchEvent( new hrfm::events::Event( hrfm::events::Event::RESIZE ) );
            _beforeWidth  = width;
            _beforeHeight = height;
            _resized = true;
        }
    }
    void DisplayNode::setSize( ivec2 size ){
        setSize( size.x, size.y );
    }
    
    vec3 DisplayNode::getScale(){
        return this->scale;
    }
    void DisplayNode::setScale( float scale ){
        this->scale = vec3( scale, scale, scale );
    }
    void DisplayNode::setScale( vec3 scale ){
        this->scale = scale;
    }
    
    Rectf DisplayNode::getBounds(){
        return Rectf( x, y, x+width, y+height );
    }
    
    Rectf DisplayNode::getDrawBounds(){
        return Rectf( 0, 0, width, height );
    }
    
    ci::vec2 DisplayNode::getAbsolutePosition(){
        return this->getGlobalPosition();
    }
    
    ci::vec2 DisplayNode::getGlobalPosition(){
        ci::vec2 pos( x, y );
        if( hasParent() ){
            return pos + _parent->getGlobalPosition();
        }else{
            return pos;
        }
    }
    
    int DisplayNode::numChildren(){
        return children.size();
    }
    
    bool DisplayNode::hasChildOf( DisplayNode * child ){
        auto itr = std::remove_if(children.begin(),children.end(),[child](DisplayNode* d)->bool{
            return d == child;
        });
        if( itr == children.end() ){
            return false;
        }
        return true;
    }
    
    DisplayNode * DisplayNode::addChild( DisplayNode * child ){
        eraseFromChildren(child);
        if( hasStage() ){ child->_setStage(_stage); }
        child->_setParent(this);
        children.push_back(child);
        return child;
    }
    
    DisplayNode * DisplayNode::addChildAt( DisplayNode * child, int index ){
        if( numChildren() < index || index < 0 ){
            throw;
        }else if( numChildren() == index ){
            return addChild(child);
        }
        std::vector<DisplayNode*>::iterator itr = children.begin() + index;
        if( *itr != child ){
            eraseFromChildren(child);
            children.insert(itr, child);
        }
        return child;
    }
    
    DisplayNode * DisplayNode::removeChild( DisplayNode * child ){
        if( eraseFromChildren(child) ){
            child->_unsetStage();
            child->_unsetParent();
        }
        return child;
    }
    
    DisplayNode * DisplayNode::removeChildAt( int index ){
        if( numChildren() <= index || index < 0 ){ throw; }
        std::vector<DisplayNode*>::iterator itr = children.begin() + index;
        children.erase(itr);
        return *itr;
    }
    
    DisplayNode * DisplayNode::removeOwn(){
        if( hasParent() ){ getParent()->removeChild(this); }
        return this;
    }
    
    void DisplayNode::update(){
        if( visible == false || ++_updateCount < _updateFequency ){
            return;
        }
        _updateCount = 0;
        if( _beforeWidth != width || _beforeHeight != height ){
            dispatchEvent( new hrfm::events::Event( hrfm::events::Event::RESIZE ) );
            _beforeWidth  = width;
            _beforeHeight = height;
        }
        _update();
        _updateChildren();
    }
    
    void DisplayNode::draw( ColorA * drawColor ){
        
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
        
        if( _enableAdditiveBlending == true ){
            ci::gl::enableAdditiveBlending();
        }else if( c.a < 1.0f ){
            ci::gl::enableAlphaBlending();
        }else{
            ci::gl::disableAlphaBlending();
        }
        ci::gl::pushModelMatrix();
            ci::gl::translate( getPosition() );
            ci::gl::rotate( rotate );
            ci::gl::color( c );
            _draw();
            _drawChildren( &c );
        ci::gl::popModelMatrix();
        ci::gl::disableAlphaBlending();
        
        _resized = false;
        
    }
    
    void DisplayNode::drawForLights(){
        ci::gl::pushModelMatrix();
            ci::gl::translate( getPosition() );
            _drawForLights();
            _drawChildrenForLights();
        ci::gl::popModelMatrix();
    }

    bool DisplayNode::isResized(){
        return _resized;
    }

    bool DisplayNode::hasParent(){
        return _parent != nullptr && _parent != NULL;
    }
    
    DisplayNode * DisplayNode::getParent(){
        return _parent;
    }
    
    bool DisplayNode::hasStage(){
        return _stage != nullptr && _stage != NULL;
    }
    
    Stage * DisplayNode::getStage(){
        return _stage;
    }
    
    void DisplayNode::setUpdateFrequency( unsigned int freq ){
        if( freq <= 0 ){
            freq = 1;
        }
        _updateFequency = _updateCount = freq;
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
    
    void DisplayNode::_updatePosition( float x, float y, float z ){
        this->x = x;
        this->y = y;
        this->z = z;
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
    void DisplayNode::_drawChildren( ColorA * drawColor ){
        if( numChildren() == 0 ) return;
        std::vector<DisplayNode*>::iterator it, end;
        for( it = children.begin(), end = children.end(); it!=end; it++ ){
            if( *it!=nullptr ){
                (*it)->draw( drawColor );
            }
        }
    }
    
    void DisplayNode::_drawForLights(){
        _draw();
    };
    void DisplayNode::_drawChildrenForLights(){
        if( numChildren() == 0 ) return;
        std::vector<DisplayNode*>::iterator it, end;
        for( it = children.begin(), end = children.end(); it!=end; it++ ){
            if( *it!=nullptr ){
                (*it)->drawForLights();
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
    
    void DisplayNode::_setStage( Stage * stage ){
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