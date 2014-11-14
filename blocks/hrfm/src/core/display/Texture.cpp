#include "Texture.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    // public:
    
    Texture::Texture(){
        DisplayNode();
    }
    
    Texture::Texture( ImageSourceRef ref ){
        Texture();
        init( ref );
    }
    
    Texture::~Texture(){}
    
    void Texture::init( ImageSourceRef ref  ){
        mTexture = gl::Texture(ref);
        if( width == 0 ){
            width = mTexture.getWidth();
        }
        if( height == 0 ){
            height = mTexture.getHeight();
        }
    }
    
    bool Texture::isDrawable(){
        if( mTexture ){
            return ( mTexture.getWidth() != 0 && mTexture.getHeight() != 0 );
        }else{
            return false;
        }
    }
    
    gl::Texture Texture::getTexture(){
        return mTexture;
    }
    
    // protected:
    
    void Texture::_update(){}
    
    void Texture::_draw(){
        if( isDrawable() ){
            gl::draw( getTexture(), getDrawBounds() );
        }
    }
    
}}