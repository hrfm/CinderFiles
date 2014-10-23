#include "OscInput.h"

using namespace ci;
using namespace std;

namespace sgmnt { namespace io{
    
    void OscInput::setup( int port ){
        _listener.setup( port );
    };
    
    void OscInput::update(){
        
        // --- update OSC. ---
        
        while( _listener.hasWaitingMessages() ) {
            
            osc::Message message;
            _listener.getNextMessage( &message );
            
            string addr = message.getAddress();
            int numArgs = message.getNumArgs();
            
            /*
             cout << "New message received" << endl;
             cout << "Address: " << addr << endl;
             cout << "Num Arg: " << numArgs << endl;
             */
            
            OscInputEvent * evt = new OscInputEvent(addr);
            evt->message = message;
            dispatchEvent( *evt );
            
            /*
             if( addr == "/points" ){
             
             }
             if( addr == "/point/start" ){
             particleController.addParticle(
             Vec2f( message.getArgAsFloat(1), message.getArgAsFloat(2) )
             );
             return;
             }
             if( addr == "/point/move" ){
             particleController.addParticle(
             Vec2f( message.getArgAsFloat(1), message.getArgAsFloat(2) )
             );
             return;
             }
             if( addr == "/point/end" ){
             particleController.addParticle(
             Vec2f( message.getArgAsFloat(1), message.getArgAsFloat(2) )
             );
             return;
             }
             */
            
        }
        
    }
    
}}