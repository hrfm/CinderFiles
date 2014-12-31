#include "SiKORGMIDIInterface.h"

namespace hrfm { namespace io{
    
    void SiKORGMIDIInterface::listen( int port ){
        SiOscInput::getInstance().addListenPort(port);
        SiOscInput::getInstance().addEventListener( "/nanokontrol", this, &SiKORGMIDIInterface::_onReceiveOscMessage );
        SiOscInput::getInstance().addEventListener( "/nanopad/on" , this, &SiKORGMIDIInterface::_onNanoPadOn );
        SiOscInput::getInstance().addEventListener( "/nanopad/off", this, &SiKORGMIDIInterface::_onNanoPadOff );
    };
    
}}