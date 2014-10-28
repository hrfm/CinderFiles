#pragma once

#include "OscEvent.h"
#include "Singleton.h"

namespace sgmnt { namespace osc{
    
    // --- OSC Manager. ---
    
    class SiOscInput : public sgmnt::utils::Singleton<SiOscInput>, public sgmnt::events::EventDispatcher{
    
    public:
        
        void addListenPort( int port );
        
        void update();
        
    private:
        
        friend class sgmnt::utils::Singleton<SiOscInput>;
        
        SiOscInput(){
            sgmnt::events::EventDispatcher();
        }
        
        map<int,ci::osc::Listener*> _listenerMap;
        
    };
    
}}