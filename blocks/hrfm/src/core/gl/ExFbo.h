#pragma once

#include "FilterBase.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Camera.h"

namespace hrfm{ namespace gl{
    
    class ExFbo{
        
    public:
        
        ExFbo( int width, int height,
               ci::CameraPersp * camera = NULL, ci::gl::Fbo::Format format = ci::gl::Fbo::Format() );
        
        ~ExFbo(){};
        
        ci::gl::FboRef getFboRef();
        
        ci::vec2 getSize();
        ci::Rectf getBounds();
        
        ci::ivec2 getAspectSize();
        ci::Rectf getAspectBounds();
        
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
        ci::Area  mTmpViewport;
        
    };
    
}}