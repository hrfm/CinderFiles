#pragma once

#include <iostream>
#include <string>

using namespace std;

namespace sgmnt { namespace events{
    
    class EventDispatcher;
    
    class Event{
        
        friend EventDispatcher;
        
	public:
    	
        Event(const std::string &type, bool bubbles = false, bool cancelable = false):_type(type), _bubbles(bubbles), _cancelable(cancelable){};
        ~Event(){};
        
        virtual Event clone();
        
		const std::string type();
        const bool bubbles();
		const bool cancelable();
        
        void * target();
        void * listener();
        
    private:
        
        const std::string _type;
        const bool _bubbles;
		const bool _cancelable;
        
        void * _target;
        void * _listener;
        
    };
    
}}