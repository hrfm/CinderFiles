#pragma once

#include "Event.h"
#include <map>
#include <list>

using namespace std;

namespace sgmnt {
    
    class EventDispatcher{
	public:
        
        EventDispatcher();
        ~EventDispatcher();
        
        void dispatchEvent(Event & event);
        
        template <class T, class E = Event>
        void addEventListener( const std::string &type, T * listener, void (T::*handler)(E&), int priority = 0, bool useWeakReference = false ){
            
            removeEventListener( type, listener, handler );
            
            _listenerList[type][priority].push_back( new EventListener<T,E>(listener,handler) );
            
        }
        
        template <class T, class E = Event>
        void removeEventListener( const std::string &type, T * listener, void (T::*handler)(E&) ){
            
            // Leave if no event registered
            if(!hasEventListener(type)){
                return;
            }
            
            // --- Remove. ---
            
            EventListener<T,E> * __listener = new EventListener<T,E>(listener,handler);
            
            // A reference to keep code clean
            map<int,list<IEventListener*>> &allListeners = _listenerList[type];
            map<int,list<IEventListener*>>::iterator it  = allListeners.begin();
            map<int,list<IEventListener*>>::iterator end = allListeners.end();
            
            // Iterate through all functions in the event, from high proproty to low
            while( it != end ){
                
                list<IEventListener*> &funcList = it->second;
                list<IEventListener*>::iterator it_f  = funcList.begin();
                list<IEventListener*>::iterator end_f = funcList.end();
                
                // Check Callbacks.
                while( it_f!=end_f ){
                    if( (*__listener) == (*it_f) ){
                        it_f = funcList.erase(it_f);
                    }else{
                        ++it_f;
                    }
                }
                
                if( funcList.empty() ){
                    it = allListeners.erase(it);
                }else{
                    ++it;
                }
                
            }
            
            if( allListeners.empty() ){
                _listenerList.erase(type);
            }
            
        };
        
        bool hasEventListener( const std::string &type );
        
	protected:
        
        void * _target;
        
    private:
        
        std::map<const std::string, std::map<int, std::list<IEventListener*> > > _listenerList;
        
    };
    
}