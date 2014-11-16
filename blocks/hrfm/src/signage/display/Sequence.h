#pragma once

#include "cinder/app/App.h"

#include "DisplayNode.h"
#include "Texture.h"
#include "MovieTexture.h"
#include "EventDispatcher.h"
#include "SiEventDispatcher.h"
#include "SequentialContents.h"
#include "Utils.h"

namespace hrfm{ namespace signage{ namespace display{
    
    class Sequence : public hrfm::events::EventDispatcher{
        
    public:
        
        Sequence();
        Sequence( hrfm::display::DisplayNode * content, float time );
        ~Sequence();
        
        void setTrigger( string trigger );
        
        hrfm::display::DisplayNode * getContentRef();
        
        void play();
        void stop();
        
        void update();
        
        void setSize( float width, float height );
        
    protected:
        
        void _onContentComplete( hrfm::events::Event * event );
        
        float _time;
        float _startedAt;
        hrfm::display::DisplayNode * _content;
        string _trigger;
        
    };

}}}