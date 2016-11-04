#include "Transition.h"
#include "SiFboFactory.h"

namespace hrfm{ namespace signage{ namespace display{
    
    //! public:
    
    Transition::Transition(){
        hrfm::events::EventDispatcher();
    };
    Transition::Transition( ci::ivec2 size, hrfm::gl::TransitionShaderBase * shader ){
        hrfm::events::EventDispatcher();
        init( size.x, size.y, shader );
    }
    Transition::Transition( int width, int height, hrfm::gl::TransitionShaderBase * shader ){
        hrfm::events::EventDispatcher();
        init( width, height, shader );
    }
    Transition::~Transition(){}
    
    void Transition::init( int width, int height, hrfm::gl::TransitionShaderBase * shader ){
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
    
    void Transition::setCurrent( hrfm::display::DisplayNode * current ){
        _current = current;
    }
    
    void Transition::setNext( hrfm::display::DisplayNode * next ){
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
        
        std::pair<ivec2,ivec2> viewport = ci::gl::getViewport();
        
        // --- Draw current into _currentFbo.
        
        _currentFbo->bindFramebuffer();
        {
            ci::gl::clear();
            if( _current ){
                _current->update();
                ci::gl::pushMatrices();
                {
                    ci::gl::viewport( ivec2(0), _currentFbo->getSize() );
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
                    ci::gl::viewport( ivec2(0), _next->size );
                    ci::gl::setMatricesWindow( _next->size.x, _next->size.y, false );
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
                ci::gl::viewport( ivec2(0), _fbo->getSize() );
                ci::gl::setMatricesWindow( _fbo->getWidth(), _fbo->getHeight(), false );
                {
                    _shader->bindShader();
                    _shader->affect( _currentFbo->getColorTexture(), _nextFbo->getColorTexture(), progress );
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
        
        ci::gl::viewport(viewport);
        
    }
    
    ci::gl::TextureRef Transition::getTexture(){
        return _fbo->getColorTexture();
    }
    
    //! protected:
    
}}}