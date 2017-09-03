#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/params/Params.h"
#include "DisplayNode.h"

namespace hrfm{ namespace display{
    
    class IStage : public DisplayNode{
    public:
        
        IStage( ivec2 initSize = ivec2(1024,1024), ci::gl::Fbo::Format format = ci::gl::Fbo::Format() ):DisplayNode(){
            this->_initialize( initSize, format );
        };
        ~IStage(){};
        
        virtual DisplayNode * addChild( DisplayNode * child );
        
        virtual void draw( bool clear = true );
        
        ci::gl::TextureRef getTexture();
        
        ci::gl::FboRef getFbo();
        
        void updateFboSize();
        
    protected:
        
        virtual void _initialize( ivec2 initSize, ci::gl::Fbo::Format format );
        virtual void _onResize( hrfm::events::Event * event );
        
        ci::gl::Fbo::Format _fboFormat;
        ci::gl::FboRef      _fboRef;
        
    };
    
}}
