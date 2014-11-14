#include "DisplayNode.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    int DisplayNode::numChildren(){
        return children.size();
    }
    
    IDrawable * DisplayNode::addChild( IDrawable * child ){
        
        eraseFromChildren(child);
        
        if( hasStage() ){
            child->_setStage(_stage);
        }
        child->_setParent(this);
        
        children.push_back(child);
        
        return child;
        
    }
    
    IDrawable * DisplayNode::removeChild( IDrawable * child ){
        
        if( eraseFromChildren(child) ){
            child->_unsetStage();
            child->_unsetParent();
        }
        
        return child;
        
    }
    
    IDrawable * DisplayNode::addChildAt( IDrawable * child, int index ){
        
        if( numChildren() < index || index < 0 ){
            
            throw;
            
        }else if( numChildren() == index ){
            
            return addChild(child);
            
        }else{
            
            std::vector<IDrawable*>::iterator itr = children.begin() + index;
            
            if( *itr != child ){
                eraseFromChildren(child);
                children.insert(itr, child);
            }
            
            return child;
            
        }
        
    }
    
    IDrawable * DisplayNode::removeChildAt( int index ){
        
        if( numChildren() <= index || index < 0 ){
            
            throw;
            
        }else{
            
            std::vector<IDrawable*>::iterator itr = children.begin() + index;
            children.erase(itr);
            return *itr;
            
        }
        
    }
    
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
    
    inline bool DisplayNode::eraseFromChildren( IDrawable * child ){
        auto itr = std::remove_if(children.begin(),children.end(),[child](IDrawable* d)->bool{
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
        
        std::vector<IDrawable*>::iterator it, end;
        for( it = children.begin(), end = children.end(); it!=end; it++ ){
            if( *it!=nullptr ){
                (*it)->update();
            }
        }
        
    }
    
    void DisplayNode::_drawChildren(){
        
        if( numChildren() == 0 ) return;
        
        std::vector<IDrawable*>::iterator it, end;
        for( it = children.begin(), end = children.end(); it!=end; it++ ){
            if( *it!=nullptr ){
                (*it)->draw();
            }
        }
        
    }
    
    void DisplayNode::_unsetStage(){
        
        IDrawable::_unsetStage();
        
        if( numChildren() == 0 ) return;
        
        std::vector<IDrawable*>::iterator it, end;
        for( it = children.begin(), end = children.end(); it!=end; it++ ){
            if( *it!=nullptr ){
                (*it)->_unsetStage();
            }
        }
        
    }
    
}}