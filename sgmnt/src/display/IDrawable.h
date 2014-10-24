#pragma once

#include "cinder/app/AppNative.h"

namespace sgmnt{ namespace display{
    
    class IDrawable{
        
    public:
        
        IDrawable(){
            x         = 0;
            y         = 0;
            width     = 0;
            height    = 0;
        };
        ~IDrawable(){};
        
        virtual void setSize( float w, float h );
        virtual void setSize( ci::Vec2f size );
        
        virtual void update();
        
        virtual void draw();
        
        float x, y;
        float width, height;
        
    protected:
        
        virtual void _update();
        virtual void _draw();
        
    };
    
}}