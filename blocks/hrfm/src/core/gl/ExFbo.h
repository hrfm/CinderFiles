#pragma once

#include "FilterBase.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Camera.h"

namespace hrfm{ namespace gl{
    
    typedef std::shared_ptr<class ExFbo> ExFboRef;
    
    class ExFbo{
        
    public:
        
        static ExFboRef create( int width, int height,
                                ci::gl::Fbo::Format format = ci::gl::Fbo::Format(), ci::CameraPersp * camera = NULL ) {
            return ExFboRef( new ExFbo( width, height, format, camera ) );
        };
        
        ExFbo( int width, int height, ci::gl::Fbo::Format format = ci::gl::Fbo::Format(), ci::CameraPersp * camera = NULL );
        ~ExFbo(){};
        
        ci::gl::FboRef getFbo();
        
        int getWidth();
        int getHeight();
        ci::ivec2 getSize();
        ci::Rectf getBounds();
        
        ci::gl::TextureRef getTexture();
        
        ExFbo * beginOffscreen( bool clear = false );
        ExFbo * endOffscreen();
        
        ExFbo * applyFilter( FilterBase * filter, bool clear = true );
        ExFbo * applyFilter( FilterBase * filter, ci::gl::TextureRef srcTexture, bool clear = true );
        
    protected:
        
        ci::gl::TextureRef _getTextureClone();
        void _beginOffscreen( ci::gl::FboRef fbo, bool clear = false, bool useAspect = false );
        void _endOffscreen();
        
        ci::gl::FboRef _fbo;
        ci::gl::FboRef _bindedFbo = NULL;
        
        bool      isBeginOffscreen;
        bool      isBeginFilter;
        std::pair<ivec2,ivec2> mTmpViewport;
        
    };
    
}}