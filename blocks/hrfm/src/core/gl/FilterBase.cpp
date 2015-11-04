#include "FilterBase.h"

namespace hrfm { namespace gl{
    
    // --- Event for OSC. ---
    
    // 最も基本的な初期化を行います.
    // どのシェーダを使うかはクラスに任せます.
    void FilterBase::setup( ivec2 size ){
        // Calcurate Aspect Ratio.
        mAspect = getAspectRatio( size );
        // Create Fbo.
        mFbo = SiFboFactory::getInstance().create( size.x, size.y, false );
    }
    
    void FilterBase::setSize( int w, int h ){
        mFbo = SiFboFactory::getInstance().create( w, h, false );
    }
    void FilterBase::setSize( ivec2 size ){
        setSize( size.x, size.y );
    }
    
    void FilterBase::affect( ci::gl::TextureRef tex, vec2 windowSize, vec2 resolution, Rectf drawRect ){
        
        if( !isEnabled() ){
            return;
        }
        
        tex->bind(0);
        begin();
        {
            mShader->uniform( "tex"       , 0 );
            mShader->uniform( "time"      , (float)ci::app::getElapsedSeconds() );
            mShader->uniform( "windowSize", windowSize );
            mShader->uniform( "resolution", resolution );
            ci::gl::drawSolidRect( drawRect );
        }
        end();
        tex->unbind();
        
    }
    
    ci::gl::TextureRef FilterBase::affect( ci::gl::TextureRef tex ){
        
        if( !isEnabled() ){
            return tex;
        }
        
        Area viewport = ci::gl::getViewport();
        
        vec2 windowSize   = mFbo.getSize();
        vec2 resolution   = vec2( mFbo.getWidth(), mFbo.getHeight() );
        
        mFbo.bindFramebuffer();
        {
            ci::gl::pushMatrices();
            {
                ci::gl::setViewport( mFbo.getBounds() );
                ci::gl::setMatricesWindow( mAspect, false );
                //--
                //ci::gl::setViewport( mFbo.getBounds() );
                //ci::gl::setMatricesWindow( mFbo.getWidth(), mFbo.getHeight(), false );
                affect( &tex, windowSize, resolution, Rectf( 0, 0, mAspect.x, mAspect.y ) );
            }
            ci::gl::popMatrices();
        }
        mFbo.unbindFramebuffer();
        
        ci::gl::setViewport( viewport );
        
        return mFbo->getColorTexture();
        
    };

    void FilterBase::draw( Rectf bounds ){
        ci::gl::draw( getTexture(), bounds );
    }
    
    ci::gl::Texture FilterBase::getTexture(){
        return mFbo.getTexture();
    }
    
    void FilterBase::bindTexture( int index ){
        mFbo.bindTexture( index );
    }
    
    void FilterBase::unbindTexture(){
        mFbo.unbindTexture();
    }
    
    int FilterBase::getWidth(){
        return mFbo.getWidth();
    }
    
    int FilterBase::getHeight(){
        return mFbo.getHeight();
    }
    
    Rectf FilterBase::getBounds(){
        return mFbo.getBounds();
    }
    
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