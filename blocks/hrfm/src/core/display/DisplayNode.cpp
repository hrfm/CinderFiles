#include "DisplayNode.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    //! public:
    
    void DisplayNode::setup(){}
    
    void DisplayNode::clear(){}
    
    Vec2i DisplayNode::getSize(){
        return Vec2f( width, height );
    }
    
    void DisplayNode::setSize( int w, int h ){
        width  = w;
        height = h;
        if( _beforeWidth != width || _beforeHeight != height ){
            dispatchEvent( new hrfm::events::Event( hrfm::events::Event::RESIZE ) );
            _beforeWidth  = width;
            _beforeHeight = height;
        }
    }
    
    void DisplayNode::setSize( Vec2i size ){
        setSize( size.x, size.y );
    }
    
    ci::fs::path DisplayNode::getSrcPath(){
        return _srcPath;
    }
    
    Rectf DisplayNode::getBounds(){
        return Rectf( x, y, x+width, y+height );
    }
    
    Rectf DisplayNode::getDrawBounds(){
        return Rectf( 0, 0, width, height );
    }
    
    Vec2f DisplayNode::getAbsolutePosition(){
        Vec2f v = Vec2f( x, y );
        DisplayNode * p = getParent();
        while( p != NULL ){
            v.x += p->x;
            v.y += p->y;
            p = p->getParent();
        }
        return v;
    }
    
    int DisplayNode::numChildren(){
        return children.size();
    }
    
    DisplayNode * DisplayNode::addChild( DisplayNode * child ){
        
        eraseFromChildren(child);
        
        if( hasStage() ){
            child->_setStage(_stage);
        }
        child->_setParent(this);
        
        children.push_back(child);
        
        return child;
        
    }
    
    DisplayNode * DisplayNode::removeChild( DisplayNode * child ){
        
        if( eraseFromChildren(child) ){
            child->_unsetStage();
            child->_unsetParent();
        }
        
        return child;
        
    }
    
    DisplayNode * DisplayNode::addChildAt( DisplayNode * child, int index ){
        
        if( numChildren() < index || index < 0 ){
            
            throw;
            
        }else if( numChildren() == index ){
            
            return addChild(child);
            
        }else{
            
            std::vector<DisplayNode*>::iterator itr = children.begin() + index;
            
            if( *itr != child ){
                eraseFromChildren(child);
                children.insert(itr, child);
            }
            
            return child;
            
        }
        
    }
    
    DisplayNode * DisplayNode::removeChildAt( int index ){
        
        if( numChildren() <= index || index < 0 ){
            
            throw;
            
        }else{
            
            std::vector<DisplayNode*>::iterator itr = children.begin() + index;
            children.erase(itr);
            return *itr;
            
        }
        
    }
    
    DisplayNode * DisplayNode::removeOwn(){
        if( hasParent() ){
            getParent()->removeChild(this);
        }
        return this;
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
    
    void DisplayNode::update(){
        
        if( visible == false || colorA.a <= 0.0f ){
            return;
        }
        
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
        if( c.a < 1.0f ){
            gl::enableAlphaBlending();
        }else{
            gl::disableAlphaBlending();
        }
        gl::pushMatrices();
        gl::translate( x, y );
        {
            gl::color( c );
            _draw();
            _drawChildren( &c );
        }
        gl::popMatrices();
        gl::disableAlphaBlending();
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
    
    //! protected:
    
    void DisplayNode::_update(){}
    
    void DisplayNode::_draw(){};
    
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
    
    void DisplayNode::_updateChildren(){
        
        if( numChildren() == 0 ) return;
        
        std::vector<DisplayNode*>::iterator it, end;
        for( it = children.begin(), end = children.end(); it!=end; it++ ){
            if( *it!=nullptr ){
                (*it)->update();
            }
        }
        
    }
    
    void DisplayNode::_drawChildren( ColorA * drawColor ){
        if( numChildren() == 0 ) return;
        std::vector<DisplayNode*>::iterator it, end;
        for( it = children.begin(), end = children.end(); it!=end; it++ ){
            if( *it!=nullptr ){
                (*it)->draw( drawColor );
            }
        }
    }
    
    ci::Vec2f DisplayNode::getGlobalPosition(){
        Vec2f pos( x, y );
        if( hasParent() ){
            return pos + _parent->getGlobalPosition();
        }else{
            return pos;
        }
    }
    
    //! protected
    
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