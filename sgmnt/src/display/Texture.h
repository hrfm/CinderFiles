#pragma once

#include "IDrawable.h"
#include "EventDispatcher.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

namespace sgmnt{ namespace display{
    
    class Texture : public IDrawable{
    
    public:
        
        Texture();
        Texture( ci::ImageSourceRef ref );
        ~Texture();
        
        virtual void init( ci::ImageSourceRef ref );
        
        virtual void setTexturePtr( ci::gl::Texture * ptr );
        
        virtual ci::gl::Texture getTexture();
        
    protected:
        
        virtual void _update();
        virtual void _draw();
        
        ci::gl::Texture * mTexturePtr;
        
    private:
        
    };
    
}}