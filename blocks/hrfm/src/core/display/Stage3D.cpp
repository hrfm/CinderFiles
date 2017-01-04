#include "Stage3D.h"

namespace hrfm{ namespace display{
    
    ci::CameraPersp Stage3D::getCameraPersp(){
        return this->_cameraPersp;
    };
        
    void Stage3D::draw( bool clear ){
        ci::gl::pushMatrices();
        {
            ci::gl::ScopedFramebuffer fbo( this->_fboRef );
            if( clear == true ){
                ci::gl::clear( ci::Color::black() );
            }
            ci::gl::ScopedViewport viewport( vec2( 0.0f ), this->_fboRef->getSize() );
            ci::gl::ScopedColor color(ci::Color::white());
            ci::gl::setMatrices(this->_cameraPersp);
            _drawChildren();
        }
        ci::gl::popMatrices();
    }
    
    ci::gl::TextureRef Stage3D::getTexture(){
        return this->_fboRef->getColorTexture();
    }
        
    ci::gl::FboRef Stage3D::getFbo(){
        return this->_fboRef;
    }
    
    //protected:
        
    void Stage3D::_initialize( ci::CameraPersp camera, ivec2 size, ci::gl::Fbo::Format format ){
        this->_cameraPersp = camera;
        this->_fboFormat   = format;
        this->size(size);
        this->addEventListener( hrfm::events::Event::RESIZE, this, &Stage3D::_onResize );
    }
    
    void Stage3D::_onResize( hrfm::events::Event * event ){
        cout << "Stage3D::_onResize" << endl;
        _fboRef = ci::gl::Fbo::create( this->width(), this->height(), this->_fboFormat );
        // --- Update Camera Aspect Ratio.
        this->_cameraPersp.setAspectRatio( (float)this->width() / (float)this->height() );
    }
    
}}