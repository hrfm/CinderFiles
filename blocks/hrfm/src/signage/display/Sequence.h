#pragma once

#include "DisplayNode.h"
#include "MovieTexture.h"
#include "ImageTexture.h"
#include "Event.h"
#include "SiEventDispatcher.h"
#include "SequentialContents.h"
#include "TriggerEvent.h"
#include "Utils.h"

namespace hrfm{ namespace signage{ namespace display{
    
    /*
     
     Sequence of ScheduledContents or SequentialContents.
     
     */
    class Sequence : public hrfm::events::EventDispatcher{
        
    public:
        
        Sequence();
        Sequence( hrfm::display::DisplayNode * content, float duration );
        ~Sequence();
        
        string getType();
        
        void setSize( float width, float height );
        
        /**
         * Set trigger when triggered on play.
         * @param trigger
         */
        void setTrigger( string trigger );
        
        /**
         * Get reference of contents.
         * @return hrfm::display::DisplayNode
         */
        hrfm::display::DisplayNode * getContentRef();
        
        /**
         * Play this sequence
         */
        void play();
        
        /**
         * Stop (Pause) this sequence.
         */
        void stop();
        
        void update();
        
    protected:
        
        void _onContentComplete( hrfm::events::Event * event );
        
        hrfm::display::DisplayNode * _content = NULL;
        
        float  _duration;
        float  _startedAt;
        
        string _type;
        string _trigger;
        
    };

}}}