#include "FilterBase.h"

namespace hrfm { namespace gl{
    
    // --- Event for OSC. ---
    
    FilterBase::FilterBase(){
        init();
    };
    
    FilterBase::FilterBase( string fragmentShader ){
        init( fragmentShader );
    };
    
    FilterBase::FilterBase( string fragmentShader, string vertexShader ){
        init( fragmentShader, vertexShader );
    };
    
    // 最も基本的な初期化を行います.
    // どのシェーダを使うかはクラスに任せます.
    void FilterBase::setup( Vec2i size ){
            
        cout << size << endl;
        
        // Calcurate Aspect Ratio.
        mAspect = getAspectRatio( size );
        
        // Create Fbo.
        mFbo = *SiFboFactory::getInstance().create( size.x, size.y, false );
        
        // Create Shader.
        cout << "FilterBase::setup => " << mFragmentShader << ", " << mVertexShader << endl;
        mShader = ShaderFactory::create( getVertexShader(), getFragmentShader() );
        
    }
        
    void FilterBase::setSize( int w, int h ){
        mFbo = *SiFboFactory::getInstance().create( w, h, false );
    }
        
    void FilterBase::setSize( Vec2i size ){
        setSize( size.x, size.y );
    }
        
    void FilterBase::affect( ci::gl::Texture * tex, Vec2f windowSize, Vec2f resolution, Rectf drawRect ){
        mShader.bind();
        {
            tex->bind(0);
            mShader.uniform( "tex"       , 0 );
            mShader.uniform( "time"      , (float)ci::app::getElapsedSeconds() );
            mShader.uniform( "windowSize", windowSize );
            mShader.uniform( "resolution", resolution );
            prepare();
            ci::gl::drawSolidRect( drawRect );
            clear();
            tex->unbind();
        }
        mShader.unbind();
    }
    
    ci::gl::Texture FilterBase::affect( ci::gl::Texture tex ){
        
        Area viewport = ci::gl::getViewport();
        
        Vec2f windowSize   = mFbo.getSize();
        Vec2f resolution   = Vec2f( mFbo.getWidth(), mFbo.getHeight() );
        
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
        
        return mFbo.getTexture();
        
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
    
    void FilterBase::init(string fragmentShader, string vertexShader ){
        mFragmentShader = fragmentShader;
        mVertexShader   = vertexShader;
        cout << "init > " << mFragmentShader << ":" << mVertexShader << endl;
    }
    
    DataSourceRef FilterBase::getVertexShader(){
        return DataLoader::load(mVertexShader);
        //return ci::app::loadResource(this->_vertexShader);
    }
    
    DataSourceRef FilterBase::getFragmentShader(){
        cout << "getFragmentShader : " << mFragmentShader << endl;
        return DataLoader::load(mFragmentShader);
        //return ci::app::loadResource(this->_fragmentShader);
    }
        
    // prepare shader, texture, and more. before drawSolidRect to FrameBuffer.
    void FilterBase::prepare(){}
    
    // clear shader, texture, and more. after drawSolidRect to FrameBuffer.
    void FilterBase::clear(){}
    
}}