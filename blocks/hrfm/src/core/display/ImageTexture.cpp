#include "ImageTexture.h"

namespace hrfm{ namespace display{
    
    // public:
    
    ImageTexture::ImageTexture(){
        TextureNode();
    }
    ImageTexture::ImageTexture( ci::fs::path path ){
        ImageTexture();
        _srcPath = path;
        init( ci::loadImage( hrfm::utils::DataLoader::load(path) ) );
    }
    ImageTexture::ImageTexture( ci::ImageSourceRef ref ){
        ImageTexture();
        init( ref );
    }
    ImageTexture::~ImageTexture(){}
    
    void ImageTexture::init( ci::ImageSourceRef ref ){
        _texture = ci::gl::Texture(ref);
        if( width == 0 ){
            width = _texture.getWidth();
        }
        if( height == 0 ){
            height = _texture.getHeight();
        }
    }
    
    // protected:
    
}}