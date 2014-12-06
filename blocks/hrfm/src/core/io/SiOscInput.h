#pragma once

#include "OscEvent.h"
#include "Singleton.h"

namespace hrfm { namespace io{
    
    // --- OSC Manager. ---
    
    class SiOscInput : public hrfm::utils::Singleton<SiOscInput>, public hrfm::events::EventDispatcher{
    
    public:
        
        void addListenPort( int port );
        
        //! ポートの番号に関わらず、なんらかのポートで OSC の受信を監視しているかどうかを取得します.
        bool listening();
        
        //! 指定したポート番号で OSC の受信を監視しているかどうかを取得します.
        bool listening( int port );
        
        void update();
        
    private:
        
        friend class hrfm::utils::Singleton<SiOscInput>;
        
        SiOscInput(){
            hrfm::events::EventDispatcher();
        }
        
        map<int,ci::osc::Listener*> _listenerMap;
        
    };
    
}}