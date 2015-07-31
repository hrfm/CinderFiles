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
        
        hrfm::display::ImageTexture * _guide = NULL;
        
        double _recentPressSpaceTime = 0.0;
        
    private:
        
        void _onPadOn( hrfm::events::KORGMIDIInterfaceEvent * event );
        
    };
    
}}