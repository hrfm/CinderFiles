#include "VboStage.h"

using namespace ci;

namespace hrfm{ namespace gl{
    
    float VboStage::getFov(){
        return _fov;
    }
    void VboStage::setFov( float fov ){
        _fov = fov;
        float far = ((width/2.0)*sin((90.0-_fov/2.0)/180.0*pi))/sin((_fov/2.0)/180.0*pi);
        camera->setPerspective( _fov, width/height, 1, far*2.0 );
        camera->lookAt( Vec3f( 0, 0, -far ), Vec3f::zero() );
        camera->setWorldUp(Vec3f(0,-1,0));
    }
    
    Vec2i VboStage::getSize(){
        return Vec2f( width, height );
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
    
    void VboStage::setSize( Vec2i size ){
        setSize( size.x, size.y );
    }

    VboNode * VboStage::addChild( VboNode * child ){
        eraseFromChildren(child);
        child->_setStage(this);
        child->_setParent(this);
        children.push_back(child);
        return child;
    }
    
    void VboStage::update(){
        
        Area viewport = ci::gl::getViewport();
        
        ci::gl::enableAlphaBlending();
        {
            _fbo.bindFramebuffer();
            {
                ci::gl::clear();
                ci::gl::pushMatrices();
                {
                    ci::gl::setViewport( (Area)_fbo.getBounds() );
                    ci::gl::setMatrices( *camera );
                    _updateChildren( camera );
                    ci::gl::color( colorA );
                    _drawChildren( camera, &colorA );
                }
                ci::gl::popMatrices();
            }
            _fbo.unbindFramebuffer();
        }
        ci::gl::disableAlphaBlending();
        
        ci::gl::setViewport(viewport);
        
    }
    
    void VboStage::draw(){
        ci::gl::draw( _fbo.getTexture(), Rectf( 0, 0, width, height ) );
    }
    
    ci::gl::Texture & VboStage::getTexture(){
        return _fbo.getTexture();
    }
    
    void VboStage::_onResize( hrfm::events::Event * event ){
        cout << "_onResize" << endl;
        ci::gl::Fbo::Format format;
        _fbo = ci::gl::Fbo( width, height, format );
        this->setFov(_fov);
    }
    
}}