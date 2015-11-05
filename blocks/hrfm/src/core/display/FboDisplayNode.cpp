#include "FboDisplayNode.h"

namespace hrfm{ namespace display{
    
    void FboDisplayNode::_draw(){
        ci::gl::draw( _fbo->getColorTexture(), getDrawBounds() );
    }
    
    ci::gl::TextureRef FboDisplayNode::getTexture(){
        return _fbo->getColorTexture();
    }
    
    // --- push matrices 周り都度書くのが面倒なのでここに書いておく.
    
    void FboDisplayNode::_bindAndPushMatrices(){
        _bindAndPushMatrices( _fbo );
    }
    
    void FboDisplayNode::_bindAndPushMatrices( ci::CameraPersp cam ){
        _bindAndPushMatrices( _fbo, cam );
    }

    void FboDisplayNode::_bindAndPushMatrices( ci::gl::FboRef fbo ){
        _tmpViewport = ci::gl::getViewport();
        fbo->bindFramebuffer();
        ci::gl::pushMatrices();
        ci::gl::viewport( ci::ivec2(0), fbo->getSize() );
        ci::gl::setMatricesWindow( fbo->getWidth(), fbo->getHeight(), false );
    }
    
    void FboDisplayNode::_bindAndPushMatrices( ci::gl::FboRef fbo, ci::CameraPersp cam ){
        _tmpViewport = ci::gl::getViewport();
        fbo->bindFramebuffer();
        ci::gl::pushMatrices();
        ci::gl::setMatrices( cam );
    }
    
    void FboDisplayNode::_unbindAndPopMatrices(){
        _unbindAndPopMatrices( _fbo );
    }
    
    void FboDisplayNode::_unbindAndPopMatrices( ci::gl::FboRef fbo ){
        ci::gl::popMatrices();
        fbo->unbindFramebuffer();
        ci::gl::viewport( _tmpViewport );
    }
    
}}