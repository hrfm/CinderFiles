#include "IDrawable.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    void IDrawable::setSize( float w, float h ){
        width  = w;
        height = h;
    }
    
    void IDrawable::setSize( Vec2f size ){
        setSize( size.x, size.y );
    }
    
    Vec2f IDrawable::getSize(){
        return Vec2f( width, height );
    }
    
    void IDrawable::setup(){}
    
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
    
}}