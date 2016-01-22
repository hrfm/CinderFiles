#pragma once

#include "EventDispatcher.h"

namespace hrfm { namespace signage { namespace events{
    
    // --- Event for OSC. ---
    
    class ScheduledContentsEvent : public hrfm::events::Event{
    
    public:
        
        ScheduledContentsEvent( const std::string &type ):Event(type){}
        ~ScheduledContentsEvent(){}
        
        Event clone(){
            ScheduledContentsEvent evt( type() );
            return evt;
        }
        
        static const string SCHEDULE_START;
        static const string SCHEDULE_END;
        
    };
    
}}}