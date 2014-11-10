#include "SiOscInput.h"

using namespace hrfm::events;

namespace hrfm { namespace io{
    
    void SiOscInput::addListenPort( int port ){
        if( _listenerMap.find( port ) == _listenerMap.end() ){
            ci::osc::Listener * listener = new ci::osc::Listener();
            listener->setup(port);
            _listenerMap[port] = listener;
        }
    };
    
    void SiOscInput::update(){
        // --- update OSC. ---
        // Iterate through all functions in the event, from high proproty to low
        for( std::map<int,ci::osc::Listener*>::iterator it=_listenerMap.begin(); it!=_listenerMap.end(); ++it ){
            ci::osc::Listener * listener = it->second;
            while( listener->hasWaitingMessages() ) {
                ci::osc::Message message;
                listener->getNextMessage( &message );
                dispatchEvent( new OscInputEvent( message.getAddress(), message ) );
            }
        }
    }
    
}}