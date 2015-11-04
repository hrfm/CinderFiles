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
        
        ci::ivec2         getSize();
        ci::ivec2         getSize( string deviceName );
        
        int               getNumCached();
        int               getNumCached( string deviceName );
        
        int               getCacheLength();
        int               getCacheLength( string deviceName );
        
        void              update();
        void              update( string deviceName );
        
        ci::CaptureRef    getCaptureRef();
        ci::CaptureRef    getCaptureRef( string deviceName );
        
        ci::Surface8uRef  getSurface();
        ci::Surface8uRef  getSurface( string deviceName );
        
        ci::gl::Texture2dRef getTexture( int cacheAt = 0 );
        ci::gl::Texture2dRef getTexture( string deviceName, int cacheAt = 0 );
        
        ci::gl::Texture2dRef getResizedTexture( ivec2 size, int cacheAt = 0 );
        ci::gl::Texture2dRef getResizedTexture( ivec2 size, string deviceName, int cacheAt = 0 );
        
        //ci::gl::Texture * getTexturePtr( int cacheAt = 0 );
        //ci::gl::Texture * getTexturePtr( string deviceName, int cacheAt = 0 );
        
        ci::gl::Texture2dRef getDiffTexture();
        ci::gl::Texture2dRef getDiffTexture( string deviceName );
        
    protected:
        
        virtual ci::gl::Texture2dRef _updateTexture( string deviceName );
        virtual ci::gl::Texture2dRef _updateDiffTexture( string deviceName );
        
        string _defaultDeviceName = "*";
        
    private:
        
        friend class hrfm::utils::Singleton<SiCaptureInput>;
        
        SiCaptureInput(){
            hrfm::events::EventDispatcher();
            _diffShader = hrfm::gl::ShaderFactory::create( fs::path("simple_vert.glsl"), fs::path("diff_frag.glsl") );
        }
        
        map<string,ci::CaptureRef> _captureRefMap;
        
        int _beforeFrame;
        map<string,int> _beforeFrameMap;
        map<string,int> _beforeTextureBeforeFrameMap;
        
        map<string,int>                          _textureCacheLengthMap;
        map<string,vector<ci::gl::Texture2dRef>> _textureCacheVectorMap;
        
        ci::gl::GlslProgRef        _diffShader;
        map<string,ci::gl::FboRef> _diffFboMap;
        
    };
    
}}