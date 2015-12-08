#include "FilterBase.h"

namespace hrfm { namespace gl{
    
    // --- Event for OSC. ---
    
    // 最も基本的な初期化を行います.
    // どのシェーダを使うかはクラスに任せます.
    void FilterBase::setup( ivec2 size ){
        // Create Fbo.
        mFbo = SiFboFactory::getInstance().create( size.x, size.y, false );
    }
    
    void FilterBase::setSize( int w, int h ){
        mFbo = SiFboFactory::getInstance().create( w, h, false );
    }
    void FilterBase::setSize( ivec2 size ){
        setSize( size.x, size.y );
    }
    
    void FilterBase::affect( ci::gl::TextureRef tex, vec2 resolution ){
        affect( tex, resolution, Rectf( 0, 0, resolution.x, resolution.y ) );
    }
    
    // なんでこれ分かれてるか忘れた・・・
    void FilterBase::affect( ci::gl::TextureRef tex, vec2 resolution, Rectf drawArea ){
        if( !isEnabled() ){
            return;
        }
        ci::gl::ScopedGlslProg    shaderScp( mShader );
        ci::gl::ScopedTextureBind tex0Scp( tex, 0 );
        prepare();
        {
            mShader->uniform( "tex"       , 0 );
            mShader->uniform( "time"      , (float)ci::app::getElapsedSeconds() );
            mShader->uniform( "resolution", resolution );
            ci::gl::drawSolidRect( drawArea, vec2(0,0), vec2(1,1) );
        }
        clear();
    }
    
    ci::gl::TextureRef FilterBase::affect( ci::gl::TextureRef tex ){
        
        if( !isEnabled() ){
            return tex;
        }
        
        ci::gl::ScopedFramebuffer scpFbo( mFbo );
        ci::gl::ScopedViewport    scpVp( ivec2(0), mFbo->getSize() );
        ci::gl::pushMatrices();
        {
            ci::gl::clear();
            ci::gl::setMatricesWindow( ci::app::toPixels( mFbo->getSize() ), true );
            affect( tex, mFbo->getSize() );
        }
        ci::gl::popMatrices();
        
        return mFbo->getColorTexture();
        
    };

    void FilterBase::draw( Rectf bounds ){
        ci::gl::draw( getTexture(), bounds );
    }
    
    ci::gl::TextureRef FilterBase::getTexture(){ return mFbo->getColorTexture(); }
    void FilterBase::bindTexture( int index ){ mFbo->bindTexture( index ); }
    void FilterBase::unbindTexture(){ mFbo->unbindTexture(); }
    
    int   FilterBase::getWidth() { return mFbo->getWidth(); }
    int   FilterBase::getHeight(){ return mFbo->getHeight(); }
    Rectf FilterBase::getBounds(){ return mFbo->getBounds(); }
    
    // protected
    
    // prepare shader, texture, and more. before drawSolidRect to FrameBuffer.
    void FilterBase::prepare(){
        ShaderBase::prepare();
    }
    
    // clear shader, texture, and more. after drawSolidRect to FrameBuffer.
    void FilterBase::clear(){
        ShaderBase::clear();
    }
    
}}