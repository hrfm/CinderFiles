#pragma once

#include "cinder/Cinder.h"
#include "OscListener.h"

using namespace ci;
using namespace std;

namespace sgmnt {
    
    // --- Event. ---
    
    class Event{
	public:
        
		Event(const std::string &type, bool bubbles = false, bool cancelable = false):
        type(type), bubbles(bubbles), cancelable(cancelable){}
        
		const std::string type;
		const bool bubbles;
		const bool cancelable;
        
        osc::Message message;
        
        /*
        const void* target;
        const unsigned int eventPhase;
        const void* currentTarget;
        */
        
    };
    
	// --- Event function callback pointer type
    typedef void (*eventFunctionPtr)(const Event &);
    
    // --- EventDispatcher. ---
    
    class EventDispatcher{
	public:
		
        bool hasEventListener(const std::string &type){
            return (eventHandlerList.find(type) != eventHandlerList.end());
        }
        
        void dispatchEvent(const Event &event){
            // Leave if no event registered
            if(!hasEventListener(event.type)){
                return;
            }
            // A reference to keep code clean
            std::map<int, std::list<eventFunctionPtr > > &allFunctions = eventHandlerList[event.type];
            // Iterate through all functions in the event, from high proproty to low
            for(std::map<int, std::list<eventFunctionPtr > >::reverse_iterator i=allFunctions.rbegin(); i!=allFunctions.rend(); ++i){
                std::list<eventFunctionPtr > &funcList = i->second;
                // Execute callbacks
                for(std::list<eventFunctionPtr >::iterator f=funcList.begin(); f!=funcList.end(); ++f){
                    (*f)(event);
                }
            }
        }
        
        void addEventListener(const std::string &type, eventFunctionPtr listener, bool useCapture = false, int priority = 0, bool useWeakReference = false){
            // Simply add the event listener to the list of listeners for the selected priority
            eventHandlerList[type][priority].push_back(listener);
        }
        
        void removeEventListener(const std::string &type, eventFunctionPtr listener, bool useCapture = false){
            // Leave if no event registered
            if(!hasEventListener(type)){
                return;
            }
            // Reference to keep the code clean
            std::map<int, std::list<eventFunctionPtr > > &allFunctions = eventHandlerList[type];
            // Since we don't know the function's priority, we'll search for it
            for(std::map<int, std::list<eventFunctionPtr > >::iterator i=allFunctions.begin(); i!=allFunctions.end(); ++i){
                // Saving a branch here: instead of checking if the callback exists let remove() do it for us
                i->second.remove(listener);
                // Remove object from the map if list gone empty to eliminate false positives
                if(i->second.empty()){
                    allFunctions.erase(i);
                }
            }
            // Remove map to eliminate false positives
            if(allFunctions.empty()){
                eventHandlerList.erase(type);
            }
        }
        
	protected:
        
        std::map<const std::string, std::map<int, std::list<eventFunctionPtr > > > eventHandlerList;
        
    };
    
}