#pragma once

#include "EventDispatcher.h"
#include "Singleton.h"

namespace sgmnt { namespace events{
    
    class SiEventDispatcher : public sgmnt::utils::Singleton<SiEventDispatcher>, public sgmnt::events::EventDispatcher{
        
    public:
        
    private:
        
        friend class sgmnt::utils::Singleton<SiEventDispatcher>;
        
        SiEventDispatcher(){}
        
    };
    
}}