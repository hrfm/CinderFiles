#include "Event.h"

namespace sgmnt { namespace events{
    
    Event Event::clone(){
        Event evt(type());
        return evt;
    }
    
    const std::string Event::type(){
        return _type;
    };
    
    void *Event::target(){
        return _target;
    };
    
    void *Event::listener(){
        return _listener;
    }
    
}}