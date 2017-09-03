#pragma once

#include "SiOscInput.h"
#include "Singleton.h"
#include "KORGMIDIInterfaceEvent.h"

namespace hrfm { namespace io{ namespace midi{
    
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
        
    };
    
}}}