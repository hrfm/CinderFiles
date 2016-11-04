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
        
        ci::CameraPersp getCameraPersp(){
            return this->_cameraPersp;
        };
        
        virtual void draw( bool clear ){
            ci::gl::pushMatrices();
            {
                ci::gl::ScopedFramebuffer fbo( this->_fboRef );
                ci::gl::ScopedViewport viewport( vec2( 0.0f ), this->_fboRef->getSize() );
                if( clear == true ){
                    ci::gl::clear( ci::Color::black() );
                }
                ci::gl::setMatrices(this->_cameraPersp);
                _drawChildren();
            }
            ci::gl::popMatrices();
        }
        
        ci::gl::TextureRef getTexture(){
            return this->_fboRef->getColorTexture();
        }
        
        ci::gl::FboRef getFbo(){
            return this->_fboRef;
        }
        
    protected:
        
        virtual void _initialize( ci::CameraPersp camera, ivec2 size, ci::gl::Fbo::Format format ){
            this->_cameraPersp = camera;
            this->_fboFormat   = format;
            this->setSize( size );
            this->addEventListener( hrfm::events::Event::RESIZE, this, &Stage3D::_onResize );
        }
        
        void _onResize( hrfm::events::Event * event ){
            cout << "Stage3D::_onResize" << endl;
            _fboRef = ci::gl::Fbo::create( this->width, this->height, this->_fboFormat );
            // --- Update Camera Aspect Ratio.
            this->_cameraPersp.setAspectRatio( (float)this->width / (float)this->height );
        }
        
        ci::CameraPersp     _cameraPersp;
        ci::gl::Fbo::Format _fboFormat;
        ci::gl::FboRef      _fboRef;
        
        std::pair<ci::ivec2,ci::ivec2> _tmpViewportOnBegin;
        
    };
    
}}