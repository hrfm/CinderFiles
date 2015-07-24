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
        
        ci::gl::Fbo * getFboPtr();
        
        ci::Vec2f getSize();
        ci::Rectf getBounds();
        ci::gl::Texture getTexture();
        ci::gl::Texture * getTexturePtr();
        
        ExFbo * beginOffscreen( bool clear = false );
        ExFbo * endOffscreen();
        
        ExFbo * applyFilter( FilterBase * filter );
        ExFbo * applyFilter( FilterBase * filter, ci::gl::Texture & srcTexture );
        ExFbo * applyFilter( FilterBase * filter, ci::gl::Texture * srcTexture );
        
    protected:
        
        ci::gl::Texture _getTextureClone();
        void _beginOffscreen( ci::gl::Fbo * fbo, bool clear = false );
        void _endOffscreen();
        
        ci::gl::Fbo * _fbo;
        ci::gl::Fbo * _bindedFbo = NULL;
        
        bool      isBeginOffscreen;
        bool      isBeginFilter;
        ci::Area  mTmpViewport;
        
    };
    
}}