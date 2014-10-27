#include "Texture.h"

using namespace ci;

namespace sgmnt{ namespace display{
    
    Texture::Texture(){
        IDrawable();
    }
    
    Texture::Texture( ImageSourceRef ref ){
        Texture();
        init( ref );
    }
    
    Texture::~Texture(){}
    
    void Texture::init( ImageSourceRef ref  ){
        if( mTexturePtr ){
            delete mTexturePtr;
        }
        mTexturePtr = new gl::Texture(ref);
        if( width == 0 ){
            width = mTexturePtr->getWidth();
        }
        if( height == 0 ){
            height = mTexturePtr->getHeight();
        }
    }
    
    gl::Texture Texture::getTexture(){
        return *mTexturePtr;
    }
    
    void Texture::_draw(){
        if( mTexturePtr ){
            gl::draw( *mTexturePtr, Rectf( 0, 0, width, height ) );
        }
    }
    
}}