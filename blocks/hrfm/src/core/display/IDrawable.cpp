#include "IDrawable.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    void IDrawable::setSize( int w, int h ){
        width  = w;
        height = h;
    }
    
    void IDrawable::setSize( Vec2i size ){
        setSize( size.x, size.y );
    }
    
    Vec2i IDrawable::getSize(){
        return Vec2f( width, height );
    }
    
    Rectf IDrawable::getBounds(){
        return Rectf( x, y, width, height );
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