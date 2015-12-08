#include "ExFbo.h"

using namespace std;
using namespace ci;

namespace hrfm{ namespace gl{
    
    ExFbo::ExFbo( int width, int height, ci::gl::Fbo::Format format, ci::CameraPersp * camera ){
        _fbo = ci::gl::Fbo::create( width, height, format );
    }
    
    ci::gl::FboRef ExFbo::getFbo(){
        return _fbo;
    }
    
    ci::vec2 ExFbo::getSize(){
        return _fbo->getSize();
    };
    
    ci::Rectf ExFbo::getBounds(){
        return _fbo->getBounds();
    };
    
    ci::gl::TextureRef ExFbo::getTexture(){
        return _fbo->getColorTexture();
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
            ci::gl::TextureRef tex = _getTextureClone();
            beginOffscreen(clear);
            filter->affect( tex, getSize() );
        }
        return this;
    }
    
    ExFbo * ExFbo::applyFilter( FilterBase * filter, ci::gl::TextureRef srcTexture, bool clear ){
        if( filter->isEnabled() ){
            beginOffscreen(clear);
            filter->affect( srcTexture, getSize() );
        }
        return this;
    }
    
    // !protected
    
    ci::gl::TextureRef ExFbo::_getTextureClone(){
        ci::gl::FboRef fbo = hrfm::gl::SiFboFactory::getInstance().create(_fbo->getWidth(),_fbo->getHeight());
        _beginOffscreen(fbo,true);
            ci::gl::draw(_fbo->getColorTexture());
        _endOffscreen();
        return fbo->getColorTexture();
    }
    
    void ExFbo::_beginOffscreen( ci::gl::FboRef fbo, bool clear, bool useAspect ){
        if( _bindedFbo != NULL && _bindedFbo != fbo ){
            _endOffscreen();
        }
        if( _bindedFbo == NULL ){
            mTmpViewport = ci::gl::getViewport();
            fbo->bindFramebuffer();
            ci::gl::pushMatrices();
            if( useAspect ){
                // 特に使っていない
                ci::gl::viewport( ivec2(0), getSize() );
                ci::gl::setMatricesWindow( _fbo->getSize(), true );
            }else{
                ci::gl::viewport( ivec2(0), getSize() );
                ci::gl::setMatricesWindow( getSize(), true );
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
            ci::gl::viewport(mTmpViewport);
            _bindedFbo = NULL;
        }
    }
    
}}