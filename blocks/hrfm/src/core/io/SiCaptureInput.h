#pragma once

#include "Singleton.h"
#include "EventDispatcher.h"
#include "cinder/gl/Texture.h"
#include "cinder/Capture.h"

namespace hrfm { namespace io{
    
    // --- OSC Manager. ---
    
    class SiCaptureInput : public hrfm::utils::Singleton<SiCaptureInput>, public hrfm::events::EventDispatcher{
        
    public:
        
        void showAllDevices();
        
        ci::CaptureRef createRef( int width, int height, string deviceName = "*" );
        
        ci::Vec2i getSize( string deviceName = "*" );
        
        ci::CaptureRef getCaptureRef( string deviceName = "*" );
        
        ci::Surface getSurface( string deviceName = "*" );
        
        ci::gl::Texture getTexture( string deviceName = "*" );
        
        void update();
        
    private:
        
        friend class hrfm::utils::Singleton<SiCaptureInput>;
        
        SiCaptureInput(){
            hrfm::events::EventDispatcher();
        }
        
        map<string,ci::CaptureRef> _captureRefMap;
        
    };
    
}}