#include "Stage.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    void Stage::setSize( int w, int h ){
        if( width != w || height != h ){
            DisplayNode::setSize( w, h );
            ci::gl::Fbo::Format format;
            _fbo = ci::gl::Fbo( w, h, format );
            dispatchEvent( new hrfm::events::Event( hrfm::events::Event::RESIZE ) );
        }
    }
    
    IDrawable * Stage::addChild( IDrawable * child ){
        eraseFromChildren(child);
        child->_setStage(this);
        child->_setParent(this);
        children.push_back(child);
        return child;
    }
    
    void Stage::update(){
        _update();
        _updateChildren();
    }
    
    void Stage::draw(){
        
        Area viewport = ci::gl::getViewport();
        
        ci::gl::enableAlphaBlending();
        
        _fbo.bindFramebuffer();
        {
            ci::gl::clear();
            ci::gl::color( ci::Color( 1.0, 1.0, 1.0) );
            ci::gl::pushMatrices();
            {
                ci::gl::setViewport( (Area)getBounds() );
                ci::gl::setMatricesWindow( width, height, false );
                {
                    _draw();
                    _drawChildren();
                }
            }
            ci::gl::popMatrices();
        }
        _fbo.unbindFramebuffer();
        
        ci::gl::translate( x, y );
        gl::draw( _fbo.getTexture(), Rectf( 0, 0, width, height ) );
        
        ci::gl::disableAlphaBlending();
        
        ci::gl::setViewport(viewport);
        
    }
    
    ci::gl::Texture Stage::getTexture(){
        return _fbo.getTexture();
    }
    
}}