#pragma once

#include "DisplayNode.h"
#include "TransitionShaderBase.h"

namespace hrfm{ namespace signage{ namespace display{
    
    class Sequence;
    
    class Transition : public hrfm::events::EventDispatcher{
        
    public:
        
        Transition();
        Transition( int width, int height, hrfm::gl::TransitionShaderBase * shader );
        Transition( ci::ivec2 size, hrfm::gl::TransitionShaderBase * shader );
        ~Transition();
        
        virtual void init( int width, int height, hrfm::gl::TransitionShaderBase * shader );
        virtual void setSize( int width, int height );
        
        virtual void prepare();
        virtual void setCurrent( hrfm::display::DisplayNode * content );
        virtual void setNext( hrfm::display::DisplayNode * content );
        virtual void start();
        virtual void start( float time );
        
        virtual bool running();
        
        virtual void update();
        
        virtual ci::gl::TextureRef getTexture();
        
    protected:
        
        ci::gl::FboRef _fbo = NULL;
        ci::gl::FboRef _currentFbo = NULL;
        ci::gl::FboRef _nextFbo = NULL;
        
        hrfm::gl::TransitionShaderBase * _shader = NULL;
        
        hrfm::display::DisplayNode * _current = NULL;
        hrfm::display::DisplayNode * _next = NULL;
        
        float _time;
        float _startedAt;
        bool  _running;
        
    };
    
}}}