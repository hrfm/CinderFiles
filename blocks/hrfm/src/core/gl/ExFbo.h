#pragma once

#include "FilterBase.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Camera.h"

namespace hrfm{ namespace gl{
    
    typedef std::shared_ptr<class ExFbo> ExFboRef;
    
    class ExFbo{
        
    public:
        
        static ExFboRef create( int width, int height,
                                ci::gl::Fbo::Format format = ci::gl::Fbo::Format(), ci::Camera * camera = NULL ) {
            return ExFboRef( new ExFbo( width, height, format, camera ) );
        };
        
        static ExFboRef create( int width, int height, ci::Camera * camera ) {
            return ExFboRef( new ExFbo( width, height, ci::gl::Fbo::Format(), camera ) );
        };
        
        ExFbo( int width, int height, ci::gl::Fbo::Format format = ci::gl::Fbo::Format(), ci::Camera * camera = NULL );
        ~ExFbo(){};
        
        ci::gl::FboRef getFbo();
        
        int getWidth();
        int getHeight();
        ci::ivec2 getSize();
        ci::Rectf getBounds();
        float getAspectRatio();
        
        void setCamera( ci::Camera * camera );
        
        ci::gl::TextureRef getTexture();
        
        ExFbo * beginOffscreen( bool clear = false );
        ExFbo * beginOffscreen( ColorA clearColor );
        ExFbo * endOffscreen();
        
        ExFbo * applyFilter( FilterBase * filter, bool clear = true );
        ExFbo * applyFilter( FilterBase * filter, ci::gl::TextureRef srcTexture, bool clear = true );
        
    protected:
        
        ci::gl::TextureRef _getTextureClone();
        void _beginOffscreen( ci::gl::FboRef fbo, bool clear = false, ColorA clearColor = ColorA(0.0,0.0,0.0,0.0) );
        void _endOffscreen();
        
        ci::gl::FboRef _fbo;
        ci::gl::FboRef _bindedFbo = NULL;
        ci::Camera * _camera = NULL;
        
        bool isBeginOffscreen;
        bool isBeginFilter;
        std::pair<ivec2,ivec2> mTmpViewport;
        
    };
    
}}