#include "ImageTexture.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    // public:
    
    ImageTexture::ImageTexture(){
        TextureNode();
    }
    ImageTexture::ImageTexture( ImageSourceRef ref ){
        ImageTexture();
        init( ref );
    }
    ImageTexture::~ImageTexture(){}
    
    void ImageTexture::init( ImageSourceRef ref ){
        _texture = gl::Texture(ref);
        if( width == 0 ){
            width = _texture.getWidth();
        }
        if( height == 0 ){
            height = _texture.getHeight();
        }
    }
    
    // protected:
    
}}