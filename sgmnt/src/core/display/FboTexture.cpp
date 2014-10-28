#include "FboTexture.h"

using namespace ci;

namespace sgmnt{ namespace display{
    
    // public:
    
    FboTexture::FboTexture(){
        sgmnt::display::Texture();
    };
    
    FboTexture::FboTexture( int width, int height ){
        FboTexture( Vec2i( width, height ) );
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
        
        cout << "------------------ " << mFbo.getBounds() << endl;
        
    };
    
    ci::gl::Fbo FboTexture::getFbo(){
        return mFbo;
    }
    
    ci::gl::Texture FboTexture::getTexture(){
        return mFbo.getTexture();
    }
    
    void FboTexture::update(){
        
        // store current viewport
        Area viewport = gl::getViewport();
        
        mFbo.bindFramebuffer();
        {
            gl::setViewport( mFbo.getBounds() );
            gl::pushMatrices();
            {
                gl::setMatrices( _getFboCameraPersp() );
                _update();
            }
            gl::popMatrices();
        }
        mFbo.unbindFramebuffer();
        
        // restore viewport.
        gl::setViewport(viewport);
        
        _updateChildren();
        
    }
    
    CameraPersp FboTexture::_getFboCameraPersp(){
        CameraPersp cam( mFbo.getWidth(), mFbo.getHeight(), 60.0f );
        cam.setPerspective( 60, mFbo.getAspectRatio(), 1, 1000 );
        cam.lookAt( Vec3f( 0.0f, 0.0f, -3.0f ), Vec3f::zero() );
        return cam;
    }
    
}}