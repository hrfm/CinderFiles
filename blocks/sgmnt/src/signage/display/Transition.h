#pragma once

#include "cinder/app/App.h"
#include "sgmnt.h"

namespace sgmnt{ namespace signage{ namespace display{
    
    class Transition : public sgmnt::display::FboTexture, public sgmnt::events::EventDispatcher{
        
    public:
        
        Transition();
        Transition( int width, int height );
        Transition( ci::Vec2i size );
        ~Transition();
        
        virtual void init( ci::Vec2i size );
        
        void setup( sgmnt::display::Texture * tex0, sgmnt::display::Texture * tex1, float time );
        
        void setSize( float width, float height );
        
        bool running();
        
    protected:
        
        float _time;
        float _startedAt;
        
        virtual void _update();
        virtual void _draw();
        
        sgmnt::display::Texture * _tex0;
        sgmnt::display::Texture * _tex1;
        
    };
    
}}}