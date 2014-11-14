#include "IDrawable.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    void IDrawable::setup(){}
    
    Vec2i IDrawable::getSize(){
        return Vec2f( _width, _height );
    }
    void IDrawable::setSize( int w, int h ){
        _width  = w;
        _height = h;
    }
    void IDrawable::setSize( Vec2i size ){
        setSize( size.x, size.y );
    }
    
    float IDrawable::getX(){
        return _x;
    }
    void IDrawable::setX( float value ){
        _x = value;
    }
    
    float IDrawable::getY(){
        return _y;
    }
    void IDrawable::setY( float value ){
        _y = value;
    }
    
    int IDrawable::getWidth(){
        return _width;
    }
    void IDrawable::setWidth( int value ){
        if( _width != value ){
            setSize( value, _height );
        }
    }
    
    int IDrawable::getHeight(){
        return _height;
    }
    void IDrawable::setHeight( int value ){
        if( _height != value ){
            setSize( _width, value );
        }
    }
    
    float IDrawable::getAlpha(){
        return _alpha;
    }
    void IDrawable::setAlpha( float value ){
        if( value < 0.0 ){
            _alpha = 0;
        }else if( 1.0 < value ){
            _alpha = 1.0;
        }else{
            _alpha = value;
        }
    }
    
    Rectf IDrawable::getBounds(){
        return Rectf( _x, _y, _width, _height );
    }
    
    Rectf IDrawable::getDrawBounds(){
        return Rectf( 0, 0, _width, _height );
    }
    
    void IDrawable::update(){
        _update();
    };
    
    void IDrawable::draw(){
        gl::pushMatrices();
        gl::translate( _x, _y );
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