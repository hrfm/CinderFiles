#pragma once

#include "hrfm.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Camera.h"
#include "DisplayNode.h"
#include "SiFboFactory.h"

namespace hrfm{ namespace display{
    
    class FboDisplayNode : public hrfm::display::DisplayNode{
        
    public:
        
        FboDisplayNode(){
            hrfm::display::DisplayNode();
            this->addEventListener( hrfm::events::Event::RESIZE, this, &FboDisplayNode::_onResize );
        }
        
        ci::gl::Texture getTexture();
        
    protected:
        
        ci::gl::Fbo _fbo;
        
        ci::gl::Fbo * _tmpFbo;
        ci::Area    _tmpViewport;
        
        virtual void _bindAndPushMatrices();
        virtual void _bindAndPushMatrices( ci::CameraPersp cam );
        virtual void _bindAndPushMatrices( ci::gl::Fbo * target );
        virtual void _bindAndPushMatrices( ci::gl::Fbo * target, ci::CameraPersp cam );
        
        virtual void _unbindAndPopMatrices();
        virtual void _unbindAndPopMatrices( ci::gl::Fbo * target );
        
        virtual void _draw();
        
        virtual void _onResize( hrfm::events::Event * event ){
            ci::gl::Fbo::Format format;
            _fbo = ci::gl::Fbo( width, height, format );
        }
        
    };

}}