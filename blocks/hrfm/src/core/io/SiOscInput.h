#pragma once

#include "OscEvent.h"
#include "Singleton.h"

namespace hrfm { namespace io{
    
    // --- OSC Manager. ---
    
    class SiOscInput : public hrfm::utils::Singleton<SiOscInput>, public hrfm::events::EventDispatcher{
    
    public:
        
        void addListenPort( int port );
        
        void update();
        
    private:
        
        friend class hrfm::utils::Singleton<SiOscInput>;
        
        SiOscInput(){
            hrfm::events::EventDispatcher();
        }
        
        map<int,ci::osc::Listener*> _listenerMap;
        
    };
    
}}