#pragma once

#include "DisplayNode.h"
#include "TransitionShaderBase.h"

namespace hrfm{ namespace signage{ namespace display{
    
    class Sequence;
    
    class Transition : public hrfm::events::EventDispatcher{
        
    public:
        
        Transition();
        Transition( int width, int height, hrfm::gl::TransitionShaderBase * shader );
        Transition( ci::Vec2i size, hrfm::gl::TransitionShaderBase * shader );
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
        
        virtual ci::gl::Texture getTexture();
        
    protected:
        
        ci::gl::Fbo * _fbo;
        ci::gl::Fbo * _currentFbo;
        ci::gl::Fbo * _nextFbo;
        
        hrfm::gl::TransitionShaderBase * _shader;
        
        hrfm::display::DisplayNode * _current;
        hrfm::display::DisplayNode * _next;
        
        float _time;
        float _startedAt;
        bool  _running;
        
    };
    
}}}