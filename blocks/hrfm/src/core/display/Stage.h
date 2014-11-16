#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "DisplayNode.h"

namespace hrfm{ namespace display{
    
    class Stage : public DisplayNode{
    public:
        Stage(){
            DisplayNode();
            _beforeWidth  = width;
            _beforeHeight = height;
        };
        ~Stage(){};
        virtual DisplayNode * addChild( DisplayNode * child );
        virtual void setSize( int w, int h );
        virtual void update();
        virtual void draw();
        ci::gl::Texture getTexture();
    private:
        ci::gl::Fbo _fbo;
        int _beforeWidth;
        int _beforeHeight;
    };
    
}}