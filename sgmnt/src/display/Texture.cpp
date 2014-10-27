#include "Texture.h"

using namespace ci;

namespace sgmnt{ namespace display{
    
    // public:
    
    Texture::Texture(){
        IDrawable();
    }
    
    Texture::Texture( ImageSourceRef ref ){
        Texture();
        init( ref );
    }
    
    Texture::~Texture(){}
    
    void Texture::init( ImageSourceRef ref  ){
        gl::Texture tex = gl::Texture(ref);
        setTexturePtr(&tex);
    }
    
    void Texture::setTexturePtr( ci::gl::Texture * ptr ){
        mTexturePtr = ptr;
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
    
    // protected:
    
    void Texture::_update(){}
    
    void Texture::_draw(){
        if( width == 0 && height == 0 ){
            gl::draw( getTexture() );
        }else{
            gl::draw( getTexture(), Rectf( 0, 0, width, height ) );
        }
    }
    
}}