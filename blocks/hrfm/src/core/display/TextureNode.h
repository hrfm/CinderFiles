#pragma once

#include "DisplayNode.h"
#include "DisplayUtil.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

namespace hrfm{ namespace display{
    
    class TextureNode : public DisplayNode{
    
    public:
        
        TextureNode();
        ~TextureNode();
        
        virtual bool isDrawable();
        
        void setTexture( ci::gl::Texture tex );
        void setLetterbox( bool flag );
        virtual ci::gl::Texture getTexture();
        
    protected:
        
        virtual void _draw();
        
        ci::gl::Texture _texture;
        
        bool _letterbox;
        
    private:
        
    };
    
}}