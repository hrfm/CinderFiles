#pragma once

#include "OscEvent.h"
#include "EventDispatcher.h"

namespace sgmnt { namespace osc{
    
    // --- OSC Manager. ---
    
    class OscInput : public sgmnt::events::EventDispatcher{
    
    public:
        
        OscInput(){ EventDispatcher(); };
        ~OscInput(){}
        
        void setup( int port );
        void update();
        
    private:
        
        ci::osc::Listener _listener;
        
    };
    
}}