#pragma once

#include "SiOscInput.h"
#include "Singleton.h"
#include "KORGMIDIInterfaceEvent.h"

namespace hrfm { namespace io{
    
    // TODO 別アプリで OSC 経由で受信しているが.
    // 最終的には MIDI で直接やりとりするようにする.
    
    class SiKORGMIDIInterface : public hrfm::utils::Singleton<SiKORGMIDIInterface>, public hrfm::events::EventDispatcher{
        
    public:
        
        void listen( int port );
        
        float nanoKontrolFader[16];
        
    private:
        
        friend class hrfm::utils::Singleton<SiKORGMIDIInterface>;
        
        SiKORGMIDIInterface(){
            hrfm::events::EventDispatcher();
            for( int i=0; i< 16;i++ ){
                nanoKontrolFader[i] = 0.0;
            }
        }
        
        // -------------
        
        void _onSlider( hrfm::events::OscInputEvent * event ){
            int idx = event->message.getArgAsInt32(0);
            nanoKontrolFader[idx] = event->message.getArgAsInt32(1) / 127.0f;
        }
        
        void _onKnob( hrfm::events::OscInputEvent * event ){
            int idx = event->message.getArgAsInt32(0);
            nanoKontrolFader[idx+8] = event->message.getArgAsInt32(1) / 127.0f;
        }
        
        // -------------
        
        void _onReceiveOscMessage( hrfm::events::OscInputEvent * event ){
            for( int i = 0; i < event->message.getNumArgs(); i++ ){
                nanoKontrolFader[i] = event->message.getArgAsInt32(i) / 127.0f;
            }
        }
        
        void _onNanoPadOn( hrfm::events::OscInputEvent * event ){
            int pad = event->message.getArgAsInt32(0);
            if( pad % 2 == 0 ){
                pad = ( pad - 36 ) / 2 + 8;
            }else{
                pad = ( pad - 37 ) / 2;
            }
            dispatchEvent( new hrfm::events::KORGMIDIInterfaceEvent( hrfm::events::KORGMIDIInterfaceEvent::PAD_ON, pad ) );
        }
        
        void _onNanoPadOff( hrfm::events::OscInputEvent * event ){
            int pad = event->message.getArgAsInt32(0);
            if( pad % 2 == 0 ){
                pad = ( pad - 36 ) / 2 + 8;
            }else{
                pad = ( pad - 37 ) / 2;
            }
            dispatchEvent( new hrfm::events::KORGMIDIInterfaceEvent( hrfm::events::KORGMIDIInterfaceEvent::PAD_OFF, pad ) );
        }
        
    };
    
}}