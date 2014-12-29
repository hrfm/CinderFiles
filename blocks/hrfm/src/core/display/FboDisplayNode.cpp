#include "FboDisplayNode.h"

namespace hrfm{ namespace display{
    
    void FboDisplayNode::_draw(){
        ci::gl::draw( _fbo.getTexture(), getDrawBounds() );
    }
    
    void FboDisplayNode::_bindAndPushMatrices(){
        
        _tmpViewport = ci::gl::getViewport();
        
        _fbo.bindFramebuffer();
        
        ci::gl::pushMatrices();
        ci::gl::setViewport( _fbo.getBounds() );
        ci::gl::setMatricesWindow( _fbo.getWidth(), _fbo.getHeight(), false );
        
    }
    
    void FboDisplayNode::_unbindAndPopMatrices(){
        
        ci::gl::popMatrices();
        
        _fbo.unbindFramebuffer();
        
        ci::gl::setViewport( _tmpViewport );
        
    }
    
}}