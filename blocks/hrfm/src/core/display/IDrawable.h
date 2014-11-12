#pragma once

#include "cinder/app/AppNative.h"

namespace hrfm{ namespace display{
    
    class IDrawable{
        
    public:
        
        IDrawable(){
            x      = 0.0f;
            y      = 0.0f;
            width  = 0.0f;
            height = 0.0f;
            alpha  = 1.0f;
        };
        ~IDrawable(){};
        
        virtual void setSize( int w, int h );
        virtual void setSize( ci::Vec2i size );
        virtual ci::Vec2i getSize();
        
        virtual ci::Rectf getBounds();
        
        virtual void setup();
        
        virtual void update();
        
        virtual void draw();
        
        float alpha;
        float x, y;
        int width, height;
        
    protected:
        
        virtual void _update();
        
        virtual void _draw();
        
    };
    
}}