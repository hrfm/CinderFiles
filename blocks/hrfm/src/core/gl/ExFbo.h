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
        ExFbo * applyFilter( FilterBase * filter, ci::gl::Texture * srcTexture = NULL );
        ExFbo * endOffscreen();
        
    protected:
        
        ci::gl::Fbo * _fbo;
        
        bool      isBeginOffscreen;
        ci::Area  mTmpViewport;
        
    };
    
}}