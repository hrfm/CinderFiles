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
        
        //virtual Event clone();
        
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
    
    class IEventListener{
    public:
        IEventListener(){};
        ~IEventListener(){};
        virtual bool operator==( IEventListener * listener ){
            cout << "IEventListener::==()" << endl;
            return this == listener;
        }
        virtual void exec( Event & event ){
            cout << "IEventListener::exec()" << endl;
        }
    };
    
    template <class T, class E = Event> class EventListener : public IEventListener{
        public :
        EventListener( T * listener, void (T::*handler)(E&) ){
            IEventListener();
            _listener = listener;
            _handler  = handler;
        }
        bool operator==( IEventListener * listener ){
            if( EventListener<T,E> * l = dynamic_cast<EventListener<T,E>*>(listener) ){
                return (*this) == l;
            }else{
                return false;
            }
        }
        bool operator==( EventListener<T,E> * listener ){
            return ( this == listener || ( listener->_listener == this->_listener && listener->_handler == this->_handler ) );
        }
        void exec( E & event ){
            (_listener->*_handler)(event);
        }
        private :
        T * _listener;
        void (T::*_handler)(E&);
    };
    
}}