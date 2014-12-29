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
        
        void              showAllDevices();
        
        ci::CaptureRef    createRef( int width, int height, string deviceName = "*", int cacheLength = 2 );
        
        ci::Vec2i         getSize( string deviceName = "*" );
        int               getNumCached( string deviceName = "*" );
        int               getCacheLength( string deviceName = "*" );
        
        void              update( string deviceName = "*" );
        ci::gl::Texture   updateTexture( string deviceName = "*" );
        ci::gl::Texture   updateDiffTexture( string deviceName = "*" );
        
        ci::CaptureRef    getCaptureRef( string deviceName = "*" );
        ci::Surface       getSurface( string deviceName = "*" );
        ci::gl::Texture   getTexture( string deviceName = "*", int cacheAt = 0 );
        ci::gl::Texture   getResizedTexture( Vec2i size, string deviceName = "*", int cacheAt = 0 );
        ci::gl::Texture * getTexturePtr( string deviceName = "*", int cacheAt = 0 );
        ci::gl::Texture   getDiffTexture( string deviceName = "*" );
        
    private:
        
        friend class hrfm::utils::Singleton<SiCaptureInput>;
        
        SiCaptureInput(){
            hrfm::events::EventDispatcher();
            _diffShader = hrfm::gl::ShaderFactory::create( "simple_vert.glsl", "diff_frag.glsl" );
        }
        
        map<string,ci::CaptureRef> _captureRefMap;
        
        int _beforeFrame;
        map<string,int> _beforeFrameMap;
        map<string,int> _beforeTextureBeforeFrameMap;
        
        map<string,int>                     _textureCacheLengthMap;
        map<string,vector<ci::gl::Texture>> _textureCacheVectorMap;
        
        ci::gl::GlslProg        _diffShader;
        map<string,ci::gl::Fbo> _diffFboMap;
        
    };
    
}}