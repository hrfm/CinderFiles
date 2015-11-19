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
    
    /* !!!!!!!
     void VboStage::addLight( ci::gl::Light * light ){
     eraseLightFromLights(light);
     _lights.push_back(light);
     }
     void VboStage::removeLight( ci::gl::Light * light ){
     eraseLightFromLights(light);
     }
     //*/
    
    void Stage::draw( bool offscreen ){
        std::pair<ivec2,ivec2> viewport = ci::gl::getViewport();
        _begin();
        {
            _draw();
            _drawChildren( &colorA );
        }
        _end();
        if( !offscreen ){
            ci::gl::translate( x, y );
            ci::gl::draw( getTexture(), getDrawBounds() );
        }
        ci::gl::viewport(viewport);
    }
    
    void Stage::begin(){
        _tmpViewportOnBegin = ci::gl::getViewport();
        _begin();
    }
    void Stage::end(){
        _end();
        ci::gl::viewport( _tmpViewportOnBegin );
    }
    
    void Stage::drawOffscreen(){
        draw(true);
    }
    
    ci::gl::TextureRef Stage::getTexture(){
        return _fbo->getColorTexture();
    }
    
    void Stage::_begin(){
        _fbo->bindFramebuffer();
        ci::gl::enableAlphaBlending();
        ci::gl::enableDepthWrite();
        ci::gl::enableDepthRead();
        ci::gl::color( colorA );
        ci::gl::pushMatrices();
        ci::gl::viewport( ivec2(0), getSize() );
        if( _autoClear == true ){
            ci::gl::clear();
        }
        if( _cameraPersp != NULL ){
            ci::gl::setMatrices( *_cameraPersp );
        }else{
            ci::gl::setMatricesWindow( width, height );
        }
    }
    void Stage::_end(){
        ci::gl::popMatrices();
        ci::gl::disableDepthRead();
        ci::gl::disableDepthWrite();
        ci::gl::disableAlphaBlending();
        _fbo->unbindFramebuffer();
    }
    
    void Stage::_onResize( hrfm::events::Event * event ){
        cout << "_onResize" << endl;
        ci::gl::Fbo::Format format;
        _fbo = ci::gl::Fbo::create( width, height, format );
        if( _cameraPersp != NULL ){
            _cameraPersp->setAspectRatio( (float)width / (float)height );
        }
    }
    
    /*!!!!!!!!!!
     inline bool Stage::eraseLightFromLights( ci::gl::Light * light ){
     auto itr = std::remove_if(_lights.begin(),_lights.end(),[light](ci::gl::Light* d)->bool{
     return d == light;
     });
     if( itr == _lights.end() ){
     return false;
     }
     _lights.erase( itr, _lights.end() );
     return true;
     }
     //*/
    
}}