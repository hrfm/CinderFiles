#pragma once

#include "Singleton.h"
#include "EventDispatcher.h"
#include "cinder/gl/Texture.h"
#include "cinder/Capture.h"

namespace sgmnt { namespace io{
    
    // --- OSC Manager. ---
    
    class SiCaptureInput : public sgmnt::utils::Singleton<SiCaptureInput>, public sgmnt::events::EventDispatcher{
        
    public:
        
        void showAllDevices();
        
        ci::CaptureRef createRef( int width, int height, string deviceName = "*" );
        
        ci::CaptureRef getCaptureRef( string deviceName );
        
        ci::Surface getSurface( string deviceName );
        
        ci::gl::Texture getTexture( string deviceName );
        
        void update();
        
    private:
        
        friend class sgmnt::utils::Singleton<SiCaptureInput>;
        
        SiCaptureInput(){
            sgmnt::events::EventDispatcher();
        }
        
        map<string,ci::CaptureRef> _captureRefMap;
        
    };
    
}}