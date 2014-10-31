#pragma once

#include "OscListener.h"
#include "EventDispatcher.h"

using namespace ci;
using namespace std;

namespace sgmnt {
    
    namespace io{
        
        // --- Event for OSC. ---
        
        class OscInput : public EventDispatcher{
        public:
            
            OscInput();
            
            void setup( int port );
            void update();
            
            protected :
            
        private:
            
            osc::Listener _listener;
            
        };
        
    }
    
}