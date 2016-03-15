#include "Stage.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    
    void Stage::setCameraPersp( ci::CameraPersp * cameraPersp ){
        _cameraPersp = cameraPersp;
    }
    
    ci::CameraPersp * Stage::getCameraPersp(){
        return _cameraPersp;
    }
    
    void Stage::setAutoClear( bool flag ){
        _autoClear = flag;
        if( _autoClear == false ){
            _fbo->bindFramebuffer();
            ci::gl::clear();
            _fbo->unbindFramebuffer();
        }
    }
    
    DisplayNode * Stage::addChild( DisplayNode * child ){
        eraseFromChildren(child);
        child->_setStage(this);
        child->_setParent(this);
        children.push_back(child);
        return child;
    }
    
    void Stage::draw( bool offscreen ){
        this->begin();
        {
            _draw();
            _drawChildren( &colorA );
        }
        this->end();
        if( !offscreen ){
            ci::gl::translate( x, y );
            ci::gl::draw( getTexture() );
        }
    }
    
    void Stage::drawOffscreen(){
        draw(true);
    }
    
    void Stage::begin(){
        _tmpViewportOnBegin = ci::gl::getViewport();
        _begin();
    }
    void Stage::end(){
        _end();
        ci::gl::viewport( _tmpViewportOnBegin );
    }
    
    ci::gl::TextureRef Stage::getTexture(){
        return this->_fbo->getColorTexture();
    }
    
    void Stage::_begin(){
        _fbo->bindFramebuffer();
        ci::gl::enableAlphaBlending();
        ci::gl::color( colorA );
        ci::gl::pushMatrices();
        if( _autoClear == true ){
            ci::gl::clear();
        }
        if( _cameraPersp != NULL ){
            ci::gl::setMatrices( *_cameraPersp );
        }else{
            ci::gl::setMatricesWindow( width, height );
        }
        ci::gl::viewport( getSize() );
    }
    void Stage::_end(){
        ci::gl::popMatrices();
        ci::gl::disableAlphaBlending();
        _fbo->unbindFramebuffer();
    }
    
    void Stage::_onResize( hrfm::events::Event * event ){
        cout << "_onResize" << endl;
        ci::gl::Fbo::Format format;
        this->_fbo = ci::gl::Fbo::create( width, height, format );
        if( _cameraPersp != NULL ){
            _cameraPersp->setAspectRatio( (float)width / (float)height );
        }
    }
    
}}