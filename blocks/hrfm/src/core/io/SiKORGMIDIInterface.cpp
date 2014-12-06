#include "SiKORGMIDIInterface.h"

namespace hrfm { namespace io{
    
    void SiKORGMIDIInterface::listen( int port ){
        SiOscInput::getInstance().addListenPort(port);
        SiOscInput::getInstance().addEventListener("/nanokontrol", this, &SiKORGMIDIInterface::_onReceiveOscMessage);
    };
    
}}