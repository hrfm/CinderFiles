#include "IDrawable.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    void IDrawable::setup(){}
    
    Vec2i IDrawable::getSize(){
        return Vec2f( width, height );
    }
    void IDrawable::setSize( int w, int h ){
        width  = w;
        height = h;
    }
    void IDrawable::setSize( Vec2i size ){
        setSize( size.x, size.y );
    }
    
    Rectf IDrawable::getBounds(){
        return Rectf( x, y, width, height );
    }
    
    Rectf IDrawable::getDrawBounds(){
        return Rectf( 0, 0, width, height );
    }
    
    void IDrawable::update(){
        _update();
    };
    
    void IDrawable::draw(){
        gl::pushMatrices();
        gl::translate( x, y );
        {
            _draw();
        }
        gl::popMatrices();
    }
    
    void IDrawable::_update(){}
    
    void IDrawable::_draw(){};
    
    bool IDrawable::hasParent(){
        return _parent != nullptr && _parent != NULL;
    }
    
    DisplayNode * IDrawable::getParent(){
        return _parent;
    }
    
    bool IDrawable::hasStage(){
        return _stage != nullptr && _stage != NULL;
    }
    
    Stage * IDrawable::getStage(){
        return _stage;
    }
    
    //! protected
    
    void IDrawable::_setParent( DisplayNode * node ){
        if( !hasParent() ){
            _parent = node;
            dispatchEvent( new hrfm::events::Event(hrfm::events::Event::ADDED) );
        }else{
            _parent = node;
        }
    }
    
    void IDrawable::_unsetParent(){
        if( hasStage() ){
            _stage = NULL;
            dispatchEvent( new hrfm::events::Event(hrfm::events::Event::REMOVED) );
        }else{
            _stage = NULL;
        }
    }
    
    void IDrawable::_setStage( Stage * stage ){
        if( !hasStage() ){
            _stage = stage;
            dispatchEvent( new hrfm::events::Event(hrfm::events::Event::ADDED_TO_STAGE) );
        }else{
            _stage = stage;
        }
    }
    
    void IDrawable::_unsetStage(){
        if( hasStage() ){
            _stage = NULL;
            dispatchEvent( new hrfm::events::Event(hrfm::events::Event::REMOVED_FROM_STAGE) );
        }else{
            _stage = NULL;
        }
    }
    
}}