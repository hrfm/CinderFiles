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
        if( mTexture ){
            delete mTexture;
        }
        mTexture = new gl::Texture(ref);
        if( width == 0 ){
            width = mTexture->getWidth();
        }
        if( height == 0 ){
            height = mTexture->getHeight();
        }
    }
    
    void Texture::_draw(){
        if( mTexture ){
            gl::draw( *mTexture, Rectf( 0, 0, width, height ) );
        }
    }
    
}}