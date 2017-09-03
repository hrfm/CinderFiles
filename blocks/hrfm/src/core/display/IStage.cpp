#include "IStage.h"

namespace hrfm{ namespace display{
    
    DisplayNode * IStage::addChild( DisplayNode * child ){
        eraseFromChildren(child);
        child->_setStage(this);
        child->_setParent(this);
        children.push_back(child);
        return child;
    }
    
    void IStage::draw( bool clear ){
        this->_fboRef->bindFramebuffer();
        {
            ci::gl::pushViewport();
            ci::gl::pushMatrices();
            {
                ci::gl::setMatricesWindow( this->size(), true );
                ci::gl::viewport( ivec2(0), this->size() );
                if( clear == true ){
                    ci::gl::clear( ci::Color::black() );
                }
                ci::gl::ScopedColor color(ci::Color::white());
                _drawChildren();
            }
            ci::gl::popMatrices();
            ci::gl::popViewport();
        }
        this->_fboRef->unbindFramebuffer();
    }
    
    ci::gl::TextureRef IStage::getTexture(){
        return this->_fboRef->getColorTexture();
    }
    
    ci::gl::FboRef IStage::getFbo(){
        return this->_fboRef;
    }
    
    void IStage::updateFboSize(){
        _fboRef = ci::gl::Fbo::create( this->width(), this->height(), this->_fboFormat );
    }
    
    void IStage::_initialize( ivec2 initSize, ci::gl::Fbo::Format format ){
        this->_fboFormat   = format;
        this->size(initSize);
        this->addEventListener( hrfm::events::Event::RESIZE, this, &IStage::_onResize );
    }
    
    void IStage::_onResize( hrfm::events::Event * event ){
        cout << "IStage::_onResize" << endl;
        this->updateFboSize();
    }
    
}}
