#pragma once

#include "hrfm.h"
#include "cinder/gl/Fbo.h"
#include "DisplayNode.h"

namespace hrfm{ namespace display{
    
    class FboDisplayNode : public hrfm::display::DisplayNode{
        
    public:
        
        FboDisplayNode(){
            hrfm::display::DisplayNode();
            this->addEventListener( hrfm::events::Event::RESIZE, this, &FboDisplayNode::_onResize );
        }
        
    protected:
        
        ci::gl::Fbo _fbo;
        ci::Area _tmpViewport;
        
        virtual void _bindAndPushMatrices();
        virtual void _unbindAndPopMatrices();
        virtual void _draw();
        
        virtual void _onResize( hrfm::events::Event * event ){
            ci::gl::Fbo::Format format;
            _fbo = ci::gl::Fbo( width, height, format );
        }
        
    };

}}