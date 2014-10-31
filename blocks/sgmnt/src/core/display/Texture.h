#pragma once

#include "DisplayNode.h"
#include "EventDispatcher.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

namespace sgmnt{ namespace display{
    
    class Texture : public DisplayNode{
    
    public:
        
        Texture();
        Texture( ci::ImageSourceRef ref );
        ~Texture();
        
        virtual void init( ci::ImageSourceRef ref );
        
        virtual bool isDrawable();
        
        virtual ci::gl::Texture getTexture();
        
    protected:
        
        virtual void _update();
        virtual void _draw();
        
        ci::gl::Texture mTexture;
        
    private:
        
    };
    
}}