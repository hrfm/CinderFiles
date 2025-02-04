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
        
        void setLetterbox( bool flag );
        void setTexture( ci::gl::Texture tex );
        virtual ci::gl::Texture getTexture();
        
    protected:
        
        virtual void _draw();
        
        bool _letterbox;
        ci::gl::Texture _texture;
        
    private:
        
    };
    
}}