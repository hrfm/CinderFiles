#pragma once

#include "cinder/app/App.h"

#include "DisplayNode.h"
#include "Texture.h"
#include "MovieTexture.h"
#include "EventDispatcher.h"
#include "Utils.h"

namespace sgmnt{ namespace signage{ namespace display{
    
    class Sequence : public sgmnt::events::EventDispatcher{
        
    public:
        
        Sequence();
        Sequence( sgmnt::display::IDrawable * content, float time );
        ~Sequence();
        
        sgmnt::display::IDrawable * getContentRef();
        
        void play();
        
        void update();
        
    protected:
        
        void _play( sgmnt::display::MovieTexture * content );
        void _play( sgmnt::display::IDrawable * content );
        
        void _update( sgmnt::display::IDrawable * content );
        
        void _onContentComplete( sgmnt::events::Event * event );
        
        float _time;
        float _startedAt;
        sgmnt::display::IDrawable * _content;
        
    };

}}}