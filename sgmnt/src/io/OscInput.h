#pragma once

#include "OscListener.h"
#include "EventDispatcher.h"

using namespace ci;
using namespace std;
using namespace sgmnt::events;

namespace sgmnt { namespace io{
    
    // --- Event for OSC. ---
    
    class OscInputEvent : public Event{
    public:
        OscInputEvent( const std::string &type ):Event(type){}
        ~OscInputEvent(){}
        osc::Message message;
    };
    
    class OscInput : public EventDispatcher{
    
    public:
        
        OscInput(){ EventDispatcher(); };
        ~OscInput(){}
        
        void setup( int port );
        void update();
        
    private:
        
        osc::Listener _listener;
        
    };
    
}}