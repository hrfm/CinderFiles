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
        
        virtual void setup();
        
        virtual void setSize( int w, int h );
        virtual void setSize( ci::Vec2i size );
        
        virtual ci::Vec2i getSize();
        virtual ci::Rectf getBounds();
        
        virtual void update();
        
        virtual void draw();
        
        virtual bool hasParent();
        virtual DisplayNode * getParent();
        
        virtual bool hasStage();
        virtual Stage * getStage();
        
        float alpha;
        float x, y;
        int width, height;
        
    protected:
        
        Stage * _stage;
        DisplayNode * _parent;
        virtual void _update();
        virtual void _draw();
        
    protected:
        
        friend class Stage;
        friend class DisplayNode;
        
        virtual void _setStage( Stage * node );
        virtual void _unsetStage();
        
        virtual void _setParent( DisplayNode * node );
        virtual void _unsetParent();
        
    };
    
}}