#pragma once

#include "cinder/app/App.h"
#include "hrfm.h"
#include "Sequence.h"

using namespace hrfm::display;
using namespace hrfm::gl;

namespace hrfm{ namespace signage{ namespace display{
    
    class Sequence;
    
    class Transition : public hrfm::events::EventDispatcher{
        
    public:
        
        Transition();
        Transition( int width, int height, TransitionShaderBase * shader );
        Transition( ci::Vec2i size, TransitionShaderBase * shader );
        ~Transition();
        
        virtual void init( int width, int height, TransitionShaderBase * shader );
        virtual void setSize( int width, int height );
        
        virtual void prepare();
        virtual void setCurrent( DisplayNode * content );
        virtual void setNext( DisplayNode * content );
        virtual void start();
        virtual void start( float time );
        
        virtual bool running();
        
        virtual void update();
        
        virtual ci::gl::Texture getTexture();
        
    protected:
        
        ci::gl::Fbo * _fbo;
        ci::gl::Fbo * _currentFbo;
        ci::gl::Fbo * _nextFbo;
        
        TransitionShaderBase * _shader;
        
        DisplayNode * _current;
        DisplayNode * _next;
        
        float _time;
        float _startedAt;
        bool  _running;
        
    };
    
}}}