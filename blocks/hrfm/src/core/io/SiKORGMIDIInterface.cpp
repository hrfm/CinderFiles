#include "SiKORGMIDIInterface.h"

namespace hrfm { namespace io{
    
    void SiKORGMIDIInterface::listen( int port ){
        
        SiOscInput::getInstance().addListenPort(port);
        
        SiOscInput::getInstance().setListener("/nanokontrol", [&]( const ci::osc::Message &msg ){
            for( int i = 0; i < msg.getNumArgs(); i++ ){
                nanoKontrolFader[i] = msg.getArgInt32(i) / 127.0f;
            }
        });
        
        SiOscInput::getInstance().setListener("/nanopad/on" , [&]( const ci::osc::Message &msg ){
            int pad = msg.getArgInt32(0);
            if( pad % 2 == 0 ){
                pad = ( pad - 36 ) / 2 + 8;
            }else{
                pad = ( pad - 37 ) / 2;
            }
            dispatchEvent( new hrfm::events::KORGMIDIInterfaceEvent( hrfm::events::KORGMIDIInterfaceEvent::PAD_ON, pad ) );
        });
        
        SiOscInput::getInstance().setListener("/nanopad/off", [&]( const ci::osc::Message &msg ){
            int pad = msg.getArgInt32(0);
            if( pad % 2 == 0 ){
                pad = ( pad - 36 ) / 2 + 8;
            }else{
                pad = ( pad - 37 ) / 2;
            }
            dispatchEvent( new hrfm::events::KORGMIDIInterfaceEvent( hrfm::events::KORGMIDIInterfaceEvent::PAD_OFF, pad ) );
        });
        
        SiOscInput::getInstance().setListener("/slider", [&]( const ci::osc::Message &msg ){
            int   idx = msg[0].int32();
            float val = msg[1].int32() / 127.0f;
            cout << "/slider " << idx << " " << val << endl;
            nanoKontrolFader[idx] = val;
        });
        
        SiOscInput::getInstance().setListener("/knob"  , [&]( const ci::osc::Message &msg ){
            int idx = msg[0].int32();
            float val = msg[1].int32() / 127.0f;
            cout << "/knob " << idx << " " << val << endl;
            nanoKontrolFader[idx+8] = msg[1].int32() / 127.0f;
        });
        
    };
    
}}