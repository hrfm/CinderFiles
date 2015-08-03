#pragma once

#include "AppBase.h"
#include "SiBPM.h"
#include "KORGMIDIInterfaceEvent.h"

namespace hrfm{ namespace app{
    
    class VJBase : public hrfm::app::AppBase{
    
    public:
        
        VJBase(){
            hrfm::app::AppBase();
        }
        ~VJBase(){}
        
        virtual void setGuide( fs::path filePath );
        virtual void setup();
        virtual void keyDown( ci::app::KeyEvent event );
        
        virtual void update();
        virtual void draw();
        
    protected:
        
        virtual void onPadOn( hrfm::events::KORGMIDIInterfaceEvent * event );
        virtual void onPadOff( hrfm::events::KORGMIDIInterfaceEvent * event );
        
        hrfm::display::ImageTexture * _guide = NULL;
        double _recentPressSpaceTime = 0.0;
        
    private:
        
        void _updateBPM();
        
    };
    
}}