#pragma once

#include "SiOscInput.h"
#include "Singleton.h"

namespace hrfm { namespace io{
    
    // TODO 別アプリで OSC 経由で受信しているが.
    // 最終的には MIDI で直接やりとりするようにする.
    
    class SiKORGMIDIInterface : public hrfm::utils::Singleton<SiKORGMIDIInterface>, public hrfm::events::EventDispatcher{
        
    public:
        
        void listen( int port );
        
        int nanoKontrolFader[16];
        
    private:
        
        friend class hrfm::utils::Singleton<SiKORGMIDIInterface>;
        
        SiKORGMIDIInterface(){
            
            hrfm::events::EventDispatcher();
            
            for( int i=0; i< 16;i++ ){
                nanoKontrolFader[i] = 0;
            }
            
        }
        
        void _onReceiveOscMessage( hrfm::events::OscInputEvent * event ){
            for( int i = 0; i < event->message.getNumArgs(); i++ ){
                nanoKontrolFader[i] = event->message.getArgAsInt32(i);
                cout << nanoKontrolFader[i] << endl;
            }
        }
        
    };
    
}}