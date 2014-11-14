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
            
            _alpha = 1.0f;
            
            _x = 0.0f;
            _y = 0.0f;
            
            _width  = 0.0f;
            _height = 0.0f;
            
        };
        ~IDrawable(){};
        
        virtual void setup();
        
        virtual ci::Vec2i getSize();
        virtual void setSize( int w, int h );
        virtual void setSize( ci::Vec2i size );
        
        virtual float getX();
        virtual void setX( float value );
        
        virtual float getY();
        virtual void setY( float value );
        
        virtual int getWidth();
        virtual void setWidth( int value );
        
        virtual int getHeight();
        virtual void setHeight( int value );
        
        virtual float getAlpha();
        virtual void setAlpha( float value );
        
        virtual ci::Rectf getBounds();
        virtual ci::Rectf getDrawBounds();
        
        virtual void update();
        virtual void draw();
        
        virtual bool hasParent();
        virtual DisplayNode * getParent();
        
        virtual bool hasStage();
        virtual Stage * getStage();
        
    protected:
        
        Stage * _stage;
        DisplayNode * _parent;
        virtual void _update();
        virtual void _draw();
        
        float _alpha;
        float _x, _y;
        int _width, _height;
        
    protected:
        
        friend class Stage;
        friend class DisplayNode;
        
        virtual void _setStage( Stage * node );
        virtual void _unsetStage();
        
        virtual void _setParent( DisplayNode * node );
        virtual void _unsetParent();
        
    };
    
}}