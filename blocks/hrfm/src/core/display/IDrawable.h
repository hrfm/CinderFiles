#pragma once

#include "cinder/app/AppNative.h"
#include "EventDispatcher.h"

namespace hrfm{ namespace display{
    
    class Stage;
    class DisplayNode;
    
    class IDrawable : public hrfm::events::EventDispatcher{
        
    public:
        
        IDrawable(){
            hrfm::events::EventDispatcher();
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
        
        virtual Stage * getStage();
        virtual bool hasStage();
        
        float alpha;
        float x, y;
        int width, height;
        
    protected:
        
        Stage * _stage;
        virtual void _update();
        virtual void _draw();
        
    protected:
        
        friend class Stage;
        friend class DisplayNode;
        
        virtual void _setStage( Stage * node );
        virtual void _unsetStage();
        
    };
    
}}