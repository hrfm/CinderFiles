#pragma once

#include "OscListener.h"
#include "EventDispatcher.h"

namespace hrfm { namespace events{
    
    // --- Event for OSC. ---
    
    class OscInputEvent : public hrfm::events::Event{
    
    public:
        
        OscInputEvent( const std::string &type, ci::osc::Message & message ):Event(type), message(message){}
        ~OscInputEvent(){}
        
        Event clone(){
            OscInputEvent evt( type(), message );
            return evt;
        }
        
        ci::osc::Message & message;
        
    };
    
}}