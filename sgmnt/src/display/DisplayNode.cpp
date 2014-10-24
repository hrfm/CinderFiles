#include "DisplayNode.h"

using namespace ci;

namespace sgmnt{ namespace display{
    
    int DisplayNode::numChildren(){
        return children.size();
    }
    
    IDrawable * DisplayNode::addChild( IDrawable * child ){
        children.remove(child);
        children.push_back(child);
        return child;
    }
    
    IDrawable * DisplayNode::removeChild( IDrawable * child ){
        children.remove(child);
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
        std::list<IDrawable*>::iterator it, end;
        for( it = children.begin(), end = children.end(); it!=end; it++ ){
            (*it)->update();
        }
    }
    
    void DisplayNode::draw(){
        _draw();
        gl::pushMatrices();
        gl::translate( x, y );
        {
            std::list<IDrawable*>::iterator it, end;
            for( it = children.begin(), end = children.end(); it!=end; it++ ){
                (*it)->draw();
            }
        }
        gl::popMatrices();
    }
    
}}