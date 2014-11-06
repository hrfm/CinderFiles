#pragma once

#include "EventDispatcher.h"

namespace sgmnt { namespace events{
    
    // --- Event for OSC. ---
    
    class TriggerEvent : public sgmnt::events::Event{
        
    public:
        
        TriggerEvent( const std::string &type, string triggerName ):Event(type), _triggerName(triggerName){}
        ~TriggerEvent(){}
        
        Event clone(){
            TriggerEvent evt( type(), triggerName() );
            return evt;
        }
        
        const string triggerName();
        
        static const string TRIGGER;
        
    protected:
        
        const string _triggerName;
        
    };
    
    
}}