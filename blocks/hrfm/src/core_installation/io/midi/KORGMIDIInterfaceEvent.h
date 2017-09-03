#pragma once

#include "Osc.h"
#include "EventDispatcher.h"

namespace hrfm { namespace io{ namespace midi{
    
    // --- Event for OSC. ---
    
    class KORGMIDIInterfaceEvent : public hrfm::events::Event{
        
    public:
        
        KORGMIDIInterfaceEvent( const std::string &type, int id ):Event(type), id(id){}
        ~KORGMIDIInterfaceEvent(){}
        
        Event clone(){
            KORGMIDIInterfaceEvent evt( type(), id );
            return evt;
        }
        
        int id;
        
        static const string PAD_ON;
        static const string PAD_OFF;
        
    };
    
}}}