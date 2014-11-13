#include "DisplayNode.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    int DisplayNode::numChildren(){
        return children.size();
    }
    
    IDrawable * DisplayNode::addChild( IDrawable * child ){
        children.remove(child);
        if( hasStage() ){
            child->_setStage(_stage);
        }
        child->_setParent(this);
        children.push_back(child);
        return child;
    }
    
    IDrawable * DisplayNode::removeChild( IDrawable * child ){
        children.remove(child);
        child->_unsetStage();
        child->_unsetParent();
        return child;
    }
    
    /*
    IDrawable * DisplayNode::addChildAt( IDrawable * child, int index ){
        if( numChildren() < index ){
            throw;
        }else if( numChildren() == index ){
            return addChild(child);
        }else{
            std::list<DisplayNode*>::iterator it = children.begin() + index;
            if( *it != child ){
                children.remove(child);
                children.insert( it, child );
            }
            return child;
        }
    }
    
    IDrawable * DisplayNode::removeChildAt( int index ){
        if( numChildren() <= index ){
            throw;
        }else{
            return (*)(children.erase( children.begin() + index );
        }
    }
    //*/
    
    void DisplayNode::update(){
        _update();
        _updateChildren();
    }
    
    void DisplayNode::draw(){
        gl::enableAlphaBlending();
        gl::pushMatrices();
        gl::translate( x, y );
        {
            _draw();
            _drawChildren();
        }
        gl::popMatrices();
    }
    
    DisplayNode * DisplayNode::removeOwn(){
        if( hasParent() ){
            getParent()->removeChild(this);
        }
        return this;
    }
    
    //! protected:
    
    void DisplayNode::_updateChildren(){
        
        if( numChildren() == 0 ) return;
        
        std::list<IDrawable*>::iterator it, end;
        for( it = children.begin(), end = children.end(); it!=end; it++ ){
            if( *it!=nullptr ){
                (*it)->update();
            }
        }
        
    }
    
    void DisplayNode::_drawChildren(){
        
        if( numChildren() == 0 ) return;
        
        std::list<IDrawable*>::iterator it, end;
        for( it = children.begin(), end = children.end(); it!=end; it++ ){
            if( *it!=nullptr ){
                (*it)->draw();
            }
        }
        
    }
    
    void DisplayNode::_unsetStage(){
        
        IDrawable::_unsetStage();
        
        if( numChildren() == 0 ) return;
        
        std::list<IDrawable*>::iterator it, end;
        for( it = children.begin(), end = children.end(); it!=end; it++ ){
            if( *it!=nullptr ){
                (*it)->_unsetStage();
            }
        }
        
    }
    
}}