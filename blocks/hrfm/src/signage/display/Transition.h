#pragma once

#include "cinder/app/App.h"
#include "hrfm.h"

namespace hrfm{ namespace signage{ namespace display{
    
    class Transition : public hrfm::display::Texture{
        
    public:
        
        Transition();
        Transition( int width, int height );
        Transition( ci::Vec2i size );
        ~Transition();
        
        virtual void init( ci::Vec2i size );
        
        void setup( hrfm::display::Texture * tex0, hrfm::display::Texture * tex1, float time );
        
        void setSize( float width, float height );
        
        bool running();
        
    protected:
        
        float _time;
        float _startedAt;
        
        virtual void _update();
        virtual void _draw();
        
        hrfm::display::Texture * _tex0;
        hrfm::display::Texture * _tex1;
        
    };
    
}}}