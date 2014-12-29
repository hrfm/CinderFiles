#pragma once

#include "Singleton.h"
#include "EventDispatcher.h"
#include "ShaderFactory.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Capture.h"
#include "hrfm.gl.h"

namespace hrfm { namespace io{
    
    // --- OSC Manager. ---
    
    class SiCaptureInput : public hrfm::utils::Singleton<SiCaptureInput>, public hrfm::events::EventDispatcher{
        
    public:
        
        void              showAllDevices();
        
        ci::CaptureRef    createRef( int width, int height, string deviceName = "", int cacheLength = 2 );
        
        void              setDefaultDeviceName( string deviceName );
        
        ci::Vec2i         getSize();
        ci::Vec2i         getSize( string deviceName );
        
        int               getNumCached();
        int               getNumCached( string deviceName );
        
        int               getCacheLength();
        int               getCacheLength( string deviceName );
        
        void              update();
        void              update( string deviceName );
        
        ci::CaptureRef    getCaptureRef();
        ci::CaptureRef    getCaptureRef( string deviceName );
        
        ci::Surface       getSurface();
        ci::Surface       getSurface( string deviceName );
        
        ci::gl::Texture   getTexture( int cacheAt = 0 );
        ci::gl::Texture   getTexture( string deviceName, int cacheAt = 0 );
        
        ci::gl::Texture   getResizedTexture( Vec2i size, int cacheAt = 0 );
        ci::gl::Texture   getResizedTexture( Vec2i size, string deviceName, int cacheAt = 0 );
        
        ci::gl::Texture * getTexturePtr( int cacheAt = 0 );
        ci::gl::Texture * getTexturePtr( string deviceName, int cacheAt = 0 );
        
        ci::gl::Texture   getDiffTexture();
        ci::gl::Texture   getDiffTexture( string deviceName );
        
    protected:
        
        virtual ci::gl::Texture _updateTexture( string deviceName );
        virtual ci::gl::Texture _updateDiffTexture( string deviceName );
        
        string _defaultDeviceName = "*";
        
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