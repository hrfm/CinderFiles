#include "TriggerEvent.h"

namespace sgmnt { namespace events{
    
    const std::string TriggerEvent::triggerName(){
        return _triggerName;
    };
    
    // --- Static Event Types.
    
    const string TriggerEvent::TRIGGER = "trigger";
    
}}