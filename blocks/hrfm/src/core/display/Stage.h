#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "DisplayNode.h"

namespace hrfm{ namespace display{
    
    class Stage : public DisplayNode{
    public:
        Stage(){
            DisplayNode();
            this->addEventListener( hrfm::events::Event::RESIZE, this, &Stage::_onResize );
        };
        ~Stage(){};
        virtual DisplayNode * addChild( DisplayNode * child );
        virtual void draw(bool offscreen = false);
        virtual void drawOffscreen();
        ci::gl::Texture & getTexture();
    private:
        ci::gl::Fbo _fbo;
        void _onResize( hrfm::events::Event * event );
    };
    
}}