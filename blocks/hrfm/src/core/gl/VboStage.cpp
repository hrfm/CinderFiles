#include "VboStage.h"

using namespace ci;

namespace hrfm{ namespace gl{
    
    float VboStage::getFov(){
        return _fov;
    }
    void VboStage::setFov( float fov ){
        _fov = fov;
        float pi = 3.14159265358979f;
        float far = ((width/2.0)*sin((90.0-_fov/2.0)/180.0*pi))/sin((_fov/2.0)/180.0*pi);
        camera->setPerspective( _fov, width/height, 1, far*2.0 );
        camera->lookAt( vec3( 0, 0, -far ), vec3( 0.0f, 0.0f, 0.0f ) );
        camera->setWorldUp(vec3(0,-1,0));
    }
    
    ivec2 VboStage::getSize(){
        return vec2( width, height );
    }
    
    void VboStage::setSize( int w, int h ){
        width  = w;
        height = h;
        if( _beforeWidth != width || _beforeHeight != height ){
            dispatchEvent( new hrfm::events::Event( hrfm::events::Event::RESIZE ) );
            _beforeWidth  = width;
            _beforeHeight = height;
        }
    }
    
    void VboStage::setSize( ivec2 size ){
        setSize( size.x, size.y );
    }
    
    VboNode * VboStage::addChild( VboNode * child ){
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
    
    void VboStage::update(){
        
        //!!!!!!! vector<ci::gl::Light*>::iterator it, end;
        
        //!!!!!!! Area viewport = ci::gl::getViewport();
        
        ci::gl::enableDepthWrite();
        ci::gl::enableDepthRead();
        
        ci::gl::enableAlphaBlending();
        {
            _fbo->bindFramebuffer();
            {
                ci::gl::clear( ColorA(0.0,0.0,0.0,0.0) );
                ci::gl::pushMatrices();
                {
                    
                    //!!!!!!! ci::gl::setViewport( (Area)_fbo->getBounds() );
                    ci::gl::setMatrices( *camera );
                    /*!!!!!!!
                    if( 0 < _lights.size() ){
                        glEnable( GL_LIGHTING );
                        for( it = _lights.begin(), end = _lights.end(); it!=end; it++ ){
                            (*it)->enable();
                        }
                    }
                    //*/
                    _updateChildren( camera );
                    _drawChildren( camera, &colorA );
                    /*!!!!!!!
                    if( 0 < _lights.size() ){
                        for( it = _lights.begin(), end = _lights.end(); it!=end; it++ ){
                            (*it)->disable();
                        }
                        glDisable( GL_LIGHTING );
                    }
                    //*/
                }
                ci::gl::popMatrices();
            }
            _fbo->unbindFramebuffer();
        }
        ci::gl::disableAlphaBlending();
        
        ci::gl::disableDepthRead();
        ci::gl::disableDepthWrite();
        
        //!!!!!! ci::gl::setViewport(viewport);
        
    }
    
    void VboStage::draw(){
        ci::gl::draw( _fbo->getColorTexture(), Rectf( 0, 0, width, height ) );
    }
    
    void VboStage::draw( ci::Rectf bounds ){
        ci::gl::draw( _fbo->getColorTexture(), bounds );
    }
    
    ci::gl::TextureRef VboStage::getTexture(){
        return _fbo->getColorTexture();
    }
    
    // --- Private
    
    /*!!!!!!!!!!
    inline bool VboStage::eraseLightFromLights( ci::gl::Light * light ){
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
    
    void VboStage::_onResize( hrfm::events::Event * event ){
        cout << "_onResize" << endl;
        ci::gl::Fbo::Format format;
        _fbo = ci::gl::Fbo::create( width, height, format );
        this->setFov(_fov);
    }
    
}}