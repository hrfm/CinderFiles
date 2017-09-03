#pragma once

#include "EventDispatcher.h"

namespace hrfm { namespace signage{ namespace display{
    
    // --- Event for OSC. ---
    
    class SequenceEvent : public hrfm::events::Event{
        
    public:
        
        SequenceEvent( const std::string &type ):Event(type){}
        ~SequenceEvent(){}
        
        Event clone(){
            SequenceEvent evt( type() );
            return evt;
        }
        
        static const string PLAY;
        
    };
    
}}}