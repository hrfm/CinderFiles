#pragma once

#include "EventDispatcher.h"

namespace sgmnt { namespace events{
    
    // --- Event for OSC. ---
    
    class TimeUtilEvent : public sgmnt::events::Event{
        
    public:
        
        TimeUtilEvent( const std::string &type, tm * timeInfo ):Event(type), timeInfo(timeInfo){}
        ~TimeUtilEvent(){}
        
        Event clone(){
            TimeUtilEvent evt( type(), timeInfo );
            return evt;
        }
        
        tm * timeInfo;
        
        static const string CHANGE_HOUR;
        static const string CHANGE_MINUTES;
        
    };
    
    
}}