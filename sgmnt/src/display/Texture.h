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
        
    protected:
        
        virtual void _draw();
        
    private:
        
        ci::gl::Texture * mTexture;
        
    };
    
}}