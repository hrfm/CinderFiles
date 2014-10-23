#pragma once

#include "OscListener.h"
#include "EventDispatcher.h"

using namespace ci;
using namespace std;

namespace sgmnt {
    
    namespace io{
        
        // --- Event for OSC. ---
        
        class OscInputEvent : public sgmnt::Event{
        public:
            OscInputEvent( const std::string &type ):sgmnt::Event(type){}
            ~OscInputEvent(){}
            osc::Message message;
        };
        
        class OscInput : public EventDispatcher{
            
            public:
                
                OscInput(){
                    EventDispatcher();
                };
                
                void setup( int port ){
                    _listener.setup( port );
                };
                
                void update(){
                    
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
                
            protected :
                
            private:
                
                osc::Listener _listener;
            
        };
        
    }
    
}