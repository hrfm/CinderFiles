#include "OscInput.h"

namespace sgmnt { namespace osc{
    
    void OscInput::setup( int port ){
        _listener.setup( port );
    };
    
    void OscInput::update(){
        
        // --- update OSC. ---
        
        while( _listener.hasWaitingMessages() ) {
            
            ci::osc::Message message;
            _listener.getNextMessage( &message );
            
            string addr = message.getAddress();
            int numArgs = message.getNumArgs();
            
            //cout << "New message received" << endl;
            //cout << "Address: " << addr << endl;
            //cout << "Num Arg: " << numArgs << endl;
            
            dispatchEvent( new OscInputEvent( addr, message ) );
            
        }
        
    }
    
}}