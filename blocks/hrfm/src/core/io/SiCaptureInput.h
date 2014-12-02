#pragma once

#include "Singleton.h"
#include "EventDispatcher.h"
#include "ShaderFactory.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
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
        
        ci::gl::Texture getDiffTexture( string deviceName = "*" );
        
    private:
        
        friend class hrfm::utils::Singleton<SiCaptureInput>;
        
        SiCaptureInput(){
            hrfm::events::EventDispatcher();
            _diffShader = hrfm::gl::ShaderFactory::create( "simple_vert.glsl", "diff_frag.glsl" );
        }
        
        int _beforeFrame;
        
        ci::gl::GlslProg _diffShader;
        
        map<string,ci::CaptureRef> _captureRefMap;
        
        map<string,ci::gl::Texture> _texMap;
        map<string,ci::gl::Fbo>     _diffFboMap;
        map<string,ci::gl::Texture> _beforeTextureMap;
        map<string,int>             _beforeTextureBeforeFrameMap;
        
    };
    
}}