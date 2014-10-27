#include "FboTexture.h"

using namespace ci;

namespace sgmnt{ namespace display{
    
    // public:
    
    FboTexture::FboTexture(){
        sgmnt::display::Texture();
        sgmnt::events::EventDispatcher();
    };
    
    FboTexture::FboTexture( int width, int height ){
        FboTexture();
        init( Vec2i( width, height ) );
    }
    
    FboTexture::FboTexture( Vec2i size ){
        FboTexture();
        init( size );
    }
    
    FboTexture::~FboTexture(){
        
    }
    
    void FboTexture::init( Vec2i size ){
        gl::Fbo::Format format;
        mFbo = gl::Fbo( size.x, size.y, format );
        setTexturePtr( &mFbo.getTexture() );
    };
    
    ci::gl::Fbo FboTexture::getFbo(){
        return mFbo;
    }
    
    void FboTexture::update(){
        Area viewport = gl::getViewport();
        gl::setViewport( (Area)mFbo.getBounds() );
        gl::pushMatrices();
        gl::setMatricesWindow( mFbo.getSize(), false );
        {
            mFbo.bindFramebuffer();
            _update();
            mFbo.unbindFramebuffer();
        }
        gl::popMatrices();
        gl::setViewport(viewport);
    }
    
}}