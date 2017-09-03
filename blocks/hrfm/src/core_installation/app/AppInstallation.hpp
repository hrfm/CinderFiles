#pragma once 

#include "App.hpp"

namespace hrfm { namespace app {
    
    class AppInstallation : public hrfm::app::App{
        
    public:
        
        AppInstallation() : hrfm::app::App(){}
        ~AppInstallation(){}
        
        virtual void setup();
        
        bool useAudio;
        bool useCapture;
        
        virtual void update();
        virtual void updateIO();
        
    protected:
        
        virtual void _setupFromXML( ci::XmlTree &xml );
        virtual void _initAudio( ci::XmlTree &xml );
        virtual void _initCapture( ci::XmlTree &xml );
        virtual void _initKORGMIDIInterface( ci::XmlTree &xml );
        
        virtual void _drawDebug();
        
    private:
        
    };
    
}}