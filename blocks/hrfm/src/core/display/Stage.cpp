#include "Stage.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    DisplayNode * Stage::addChild( DisplayNode * child ){
        eraseFromChildren(child);
        child->_setStage(this);
        child->_setParent(this);
        children.push_back(child);
        return child;
    }
    
    void Stage::setAutoClear( bool flag ){
        _autoClear = flag;
        if( _autoClear == false ){
            _fbo.bindFramebuffer();
            ci::gl::clear();
            _fbo.unbindFramebuffer();
        }
    }
    
    void Stage::draw( bool offscreen ){
        
        Area viewport = ci::gl::getViewport();
        
        ci::gl::enableAlphaBlending();
        
        _fbo.bindFramebuffer();
        {
            if( _autoClear == true ){
                ci::gl::clear();
            }
            ci::gl::color( colorA );
            ci::gl::pushMatrices();
            {
                ci::gl::setViewport( (Area)getBounds() );
                ci::gl::setMatricesWindow( width, height, false );
                {
                    _draw();
                    _drawChildren( &colorA );
                }
            }
            ci::gl::popMatrices();
        }
        _fbo.unbindFramebuffer();
        
        if( !offscreen ){
            ci::gl::translate( x, y );
            gl::draw( _fbo.getTexture(), getDrawBounds() );
        }
        
        ci::gl::disableAlphaBlending();
        
        ci::gl::setViewport(viewport);
        
    }
    
    void Stage::drawOffscreen(){
        draw(true);
    }
    
    ci::gl::Texture & Stage::getTexture(){
        return _fbo.getTexture();
    }
    
    void Stage::_onResize( hrfm::events::Event * event ){
        cout << "_onResize" << endl;
        ci::gl::Fbo::Format format;
        _fbo = ci::gl::Fbo( width, height, format );
    }
    
}}