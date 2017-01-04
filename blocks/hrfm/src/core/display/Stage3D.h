#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "IStage.h"

namespace hrfm{ namespace display{
    
    typedef std::shared_ptr<class Stage3D> Stage3DRef;
    
    class Stage3D : public IStage{
    public:
        
        static Stage3DRef create( ci::CameraPersp camera, ivec2 size = ivec2(1024,1024), ci::gl::Fbo::Format format = ci::gl::Fbo::Format() ){
            return Stage3DRef( new Stage3D(camera,size,format) );
        }
        
        Stage3D( ci::CameraPersp camera, ivec2 size, ci::gl::Fbo::Format format ):IStage(){
            this->_initialize( camera, size, format );
        };
        ~Stage3D(){};
        
        ci::CameraPersp getCameraPersp();
        
        virtual void draw( bool clear );
        
        ci::gl::TextureRef getTexture();
        
        ci::gl::FboRef getFbo();
        
    protected:
        
        virtual void _initialize( ci::CameraPersp camera, ivec2 size, ci::gl::Fbo::Format format );
        void _onResize( hrfm::events::Event * event );
        
        ci::CameraPersp     _cameraPersp;
        ci::gl::Fbo::Format _fboFormat;
        ci::gl::FboRef      _fboRef;
        
        std::pair<ci::ivec2,ci::ivec2> _tmpViewportOnBegin;
        
    };
    
}}