#include "Event.h"

namespace sgmnt { namespace events{
    
    Event Event::clone(){
        Event evt(type());
        return evt;
    }
    
    const std::string Event::type(){
        return _type;
    };
    
    const bool Event::cancelable(){
        return _cancelable;
    };
    
    void Event::cancel(){
        _canceled = true;
    }
    
    void *Event::target(){
        return _target;
    };
    
    void *Event::listener(){
        return _listener;
    }
    
}}