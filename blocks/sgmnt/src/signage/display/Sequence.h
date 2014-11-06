#pragma once

#include "cinder/app/App.h"

#include "DisplayNode.h"
#include "Texture.h"
#include "MovieTexture.h"
#include "EventDispatcher.h"
#include "SiEventDispatcher.h"
#include "SequentialContents.h"
#include "Utils.h"

namespace sgmnt{ namespace signage{ namespace display{
    
    class Sequence : public sgmnt::events::EventDispatcher{
        
    public:
        
        Sequence();
        Sequence( sgmnt::display::IDrawable * content, float time );
        ~Sequence();
        
        void setTrigger( string trigger );
        
        sgmnt::display::IDrawable * getContentRef();
        
        void play();
        void stop();
        
        void update();
        
        void setSize( float width, float height );
        
    protected:
        
        void _onContentComplete( sgmnt::events::Event * event );
        
        float _time;
        float _startedAt;
        sgmnt::display::IDrawable * _content;
        string _trigger;
        
    };

}}}