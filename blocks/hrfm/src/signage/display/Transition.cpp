#include "Transition.h"
#include "SiFboFactory.h"

using namespace hrfm::display;
using namespace hrfm::events;
using namespace hrfm::gl;

namespace hrfm{ namespace signage{ namespace display{
    
    //! public:
    
    Transition::Transition(){
        hrfm::events::EventDispatcher();
    };
    Transition::Transition( ci::Vec2i size, TransitionShaderBase * shader ){
        hrfm::events::EventDispatcher();
        init( size.x, size.y, shader );
    }
    Transition::Transition( int width, int height, TransitionShaderBase * shader ){
        hrfm::events::EventDispatcher();
        init( width, height, shader );
    }
    Transition::~Transition(){}
    
    void Transition::init( int width, int height, TransitionShaderBase * shader ){
        _shader = shader;
        setSize( width, height );
    }
    
    void Transition::setSize( int width, int height ){
        
        _fbo        = hrfm::gl::SiFboFactory::getInstance().create( width, height, false );
        _currentFbo = hrfm::gl::SiFboFactory::getInstance().create( width, height, false );
        _nextFbo    = hrfm::gl::SiFboFactory::getInstance().create( width, height, false );
        
        _shader->setSize( width, height );
        
    }
    
    void Transition::prepare(){
        _current = NULL;
        _next    = NULL;
    }
    
    void Transition::setCurrent( DisplayNode * current ){
        _current = current;
    }
    
    void Transition::setNext( DisplayNode * next ){
        _next = next;
    }
    
    void Transition::start(){
        start( _shader->getInterval() );
    }
    
    void Transition::start( float time ){
        if( 10.0 < time ){
            time = 10.0;
        }else if( time < 0 ){
            time = 0;
        }
        cout << "Transition::start(" << time << ")" << endl;
        _time      = time;
        _startedAt = ci::app::getElapsedSeconds();
        _running   = true;
    }
    
    bool Transition::running(){
        return _running;
    }
    
    void Transition::update(){
        
        float elapsed = ci::app::getElapsedSeconds() - _startedAt;
        
        float progress = elapsed / _time;
        if( 1.0 < progress ){
            progress = 1.0;
        }
        
        Area viewport = ci::gl::getViewport();
        
        // --- Draw current into _currentFbo.
        
        _currentFbo->bindFramebuffer();
        {
            ci::gl::clear();
            if( _current ){
                _current->update();
                ci::gl::pushMatrices();
                {
                    ci::gl::setViewport( (Area)_currentFbo->getBounds() );
                    ci::gl::setMatricesWindow( _currentFbo->getWidth(), _currentFbo->getHeight(), false );
                    {
                        ci::gl::color( ColorA( 1.0, 1.0, 1.0, 1.0 ) );
                        _current->draw();
                    }
                }
                ci::gl::popMatrices();
            }
        }
        _currentFbo->unbindFramebuffer();
        
        // --- Draw next into _nextFbo.
        
        _nextFbo->bindFramebuffer();
        {
            ci::gl::clear();
            if( _next ){
                _next->update();
                ci::gl::pushMatrices();
                {
                    ci::gl::setViewport( (Area)_next->getBounds() );
                    ci::gl::setMatricesWindow( _next->width, _next->height, false );
                    {
                        ci::gl::color( ColorA( 1.0, 1.0, 1.0, 1.0 ) );
                        _next->draw();
                    }
                }
                ci::gl::popMatrices();
            }
        }
        _nextFbo->unbindFramebuffer();
        
        // --- Draw output into _fbo;
        
        _shader->prepare();
        
        _fbo->bindFramebuffer();
        {
            ci::gl::clear();
            ci::gl::pushMatrices();
            {
                ci::gl::setViewport( (Area)_fbo->getBounds() );
                ci::gl::setMatricesWindow( _fbo->getWidth(), _fbo->getHeight(), false );
                {
                    _shader->bindShader();
                    _shader->affect( _currentFbo->getTexture(), _nextFbo->getTexture(), progress );
                    _shader->unbindShader();
                    /*
                    ci::gl::enableAdditiveBlending();
                    {
                        ci::gl::color( ColorA( 1.0, 1.0, 1.0, 1.0 - progress ) );
                        ci::gl::draw( _currentFbo->getTexture(), _fbo->getBounds() );
                        ci::gl::color( ColorA( 1.0, 1.0, 1.0, progress ) );
                        ci::gl::draw( _nextFbo->getTexture(), _fbo->getBounds() );
                    }
                    ci::gl::disableAlphaBlending();
                    */
                }
            }
            ci::gl::popMatrices();
        }
        _fbo->unbindFramebuffer();
        
        if( _time < elapsed ){
            _running = false;
            dispatchEvent( new hrfm::events::Event( hrfm::events::Event::COMPLETE ) );
        }
        
        ci::gl::setViewport(viewport);
        
    }
    
    ci::gl::Texture Transition::getTexture(){
        return _fbo->getTexture();
    }
    
    //! protected:
    
}}}