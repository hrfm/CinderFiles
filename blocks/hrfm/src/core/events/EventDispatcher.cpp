#include "EventDispatcher.h"

using namespace std;

namespace hrfm { namespace events {
    
    EventDispatcher::EventDispatcher(){
        _target = this;
    };
    
    EventDispatcher::~EventDispatcher(){
        _target = NULL;
        _listenerList.clear();
    };
    
    void EventDispatcher::dispatchEvent(Event * event){
        
        // Leave if no event registered
        if(!hasEventListener(event->type())){
            return;
        }
        
        event->_target = this;
        
        // A reference to keep code clean
        std::map<int, std::list<IEventListener*> > &list = _listenerList[event->type()];
        
        // Iterate through all functions in the event, from high proproty to low
        for( std::map<int, std::list<IEventListener*> >::reverse_iterator it=list.rbegin(); it!=list.rend(); ++it ){
            std::list<IEventListener*> &funcList = it->second;
            // Execute callbacks
            for(std::list<IEventListener*>::iterator it_f=funcList.begin(), end_f=funcList.end(); it_f!=end_f; ++it_f){
                
                (*it_f)->exec(event);
                
                // return immediately if cancel() called.
                if( event->_canceled == true ){
                    return;
                }
                
            }
        }
        
    };
    
    bool EventDispatcher::hasEventListener(const std::string &type){
        return ( _listenerList.find(type) != _listenerList.end() );
    };
    
}}