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
    
    ci::gl::Texture ExFbo::getTexture(){
        return _fbo->getTexture();
    }
    
    ci::gl::Texture * ExFbo::getTexturePtr(){
        return &(_fbo->getTexture());
    }
    
    ExFbo * ExFbo::beginOffscreen( bool clear ){
        if( isBeginOffscreen == false ){
            mTmpViewport = ci::gl::getViewport();
            _fbo->bindFramebuffer();
            ci::gl::pushMatrices();
            ci::gl::setViewport( (Area)getBounds() );
            ci::gl::setMatricesWindow( getSize(), false );
            if( clear == true){
                ci::gl::clear();
            }
            isBeginOffscreen = true;
        }
        return this;
    }
    
    ExFbo * ExFbo::applyFilter( FilterBase * filter ){
        beginOffscreen();
        filter->affect( getTexturePtr(), getSize(), getSize(), getBounds() );
        return this;
    }
    
    ExFbo * ExFbo::applyFilter( FilterBase * filter, ci::gl::Texture & srcTexture ){
        beginOffscreen();
        filter->affect( &srcTexture, getSize(), getSize(), getBounds() );
        return this;
    }
    ExFbo * ExFbo::applyFilter( FilterBase * filter, ci::gl::Texture * srcTexture ){
        beginOffscreen();
        filter->affect( srcTexture, getSize(), getSize(), getBounds() );
        return this;
    }
    
    ExFbo * ExFbo::endOffscreen(){
        if( isBeginOffscreen == true ){
            ci::gl::popMatrices();
            _fbo->unbindFramebuffer();
            ci::gl::setViewport(mTmpViewport);
            isBeginOffscreen = false;
        }
        return this;
    }
    
}}