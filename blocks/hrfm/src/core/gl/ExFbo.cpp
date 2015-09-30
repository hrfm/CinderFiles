#include "ExFbo.h"

using namespace std;
using namespace ci;

namespace hrfm{ namespace gl{
    
    ExFbo::ExFbo( int width, int height, ci::CameraPersp * camera, ci::gl::Fbo::Format format ){
        _fbo = new ci::gl::Fbo( width, height, format );
    }
    
    ci::gl::Fbo * ExFbo::getFboPtr(){
        return _fbo;
    }
    
    ci::Vec2f ExFbo::getSize(){
        return _fbo->getSize();
    };
    
    ci::Rectf ExFbo::getBounds(){
        return _fbo->getBounds();
    };
    
    ci::Vec2i ExFbo::getAspectSize(){
        return getAspectRatio(getSize());
    }
    ci::Rectf ExFbo::getAspectBounds(){
        Vec2i aspect = getAspectSize();
        return Rectf( 0, 0, aspect.x, aspect.y );
    }
    
    ci::gl::Texture ExFbo::getTexture(){
        return _fbo->getTexture();
    }
    
    ci::gl::Texture * ExFbo::getTexturePtr(){
        return &(_fbo->getTexture());
    }
    
    ExFbo * ExFbo::beginOffscreen( bool clear ){
        _beginOffscreen( _fbo, clear );
        return this;
    }
    
    ExFbo * ExFbo::endOffscreen(){
        _endOffscreen();
        return this;
    }
    
    // =======================================================================================
    // =====================================
    // === FILTER ===
    
    ExFbo * ExFbo::applyFilter( FilterBase * filter, bool clear ){
        if( filter->isEnabled() ){
            ci::gl::Texture tex = _getTextureClone();
            beginOffscreen(clear);
            filter->affect( &tex, getSize(), getSize(), getBounds() );
        }
        return this;
    }
    
    ExFbo * ExFbo::applyFilter( FilterBase * filter, ci::gl::Texture & srcTexture, bool clear ){
        if( filter->isEnabled() ){
            beginOffscreen(clear);
            filter->affect( &srcTexture, getSize(), getSize(), getBounds() );
        }
        return this;
    }
    ExFbo * ExFbo::applyFilter( FilterBase * filter, ci::gl::Texture * srcTexture, bool clear ){
        if( filter->isEnabled() ){
            beginOffscreen(clear);
            filter->affect( srcTexture, getSize(), getSize(), getBounds() );
        }
        return this;
    }
    
    // !protected
    
    ci::gl::Texture ExFbo::_getTextureClone(){
        ci::gl::Fbo * fbo = hrfm::gl::SiFboFactory::getInstance().create(_fbo->getWidth(),_fbo->getHeight());
        _beginOffscreen(fbo,true);
            ci::gl::draw(_fbo->getTexture());
        _endOffscreen();
        return fbo->getTexture();
    }
    
    void ExFbo::_beginOffscreen( ci::gl::Fbo * fbo, bool clear, bool useAspect ){
        if( _bindedFbo != NULL && _bindedFbo != fbo ){
            _endOffscreen();
        }
        if( _bindedFbo == NULL ){
            mTmpViewport = ci::gl::getViewport();
            fbo->bindFramebuffer();
            ci::gl::pushMatrices();
            if( useAspect ){
                // 特に使っていない
                ci::gl::setViewport( (Area)getAspectBounds() );
                ci::gl::setMatricesWindow( getAspectSize(), false );
            }else{
                ci::gl::setViewport( (Area)getBounds() );
                ci::gl::setMatricesWindow( getSize(), false );
            }
            if( clear == true){
                ci::gl::clear( ColorA(0.0,0.0,0.0,0.0) );
            }
            _bindedFbo = fbo;
        }
    }
    
    void ExFbo::_endOffscreen(){
        if( _bindedFbo != NULL ){
            ci::gl::popMatrices();
            _bindedFbo->unbindFramebuffer();
            ci::gl::setViewport(mTmpViewport);
            _bindedFbo = NULL;
        }
    }
    
}}