#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "DisplayNode.h"

namespace hrfm{ namespace display{
    
    class Stage : public DisplayNode{
    public:
        Stage(){
            DisplayNode();
        };
        ~Stage(){};
        virtual IDrawable * addChild( IDrawable * child );
        virtual void setSize( int w, int h );
        virtual void update();
        virtual void draw();
        ci::gl::Texture getTexture();
    private:
        ci::gl::Fbo _fbo;
    };
    
}}