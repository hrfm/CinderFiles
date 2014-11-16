#pragma once

#include "TextureNode.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

namespace hrfm{ namespace display{
    
    class ImageTexture : public TextureNode{
        
    public:
        
        ImageTexture();
        ImageTexture( ci::ImageSourceRef ref );
        ~ImageTexture();
        
        void init( ci::ImageSourceRef ref );
        
    protected:
        
    private:
        
    };
    
}}