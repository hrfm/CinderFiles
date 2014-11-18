#pragma once

#include "cinder/app/App.h"
#include "hrfm.h"
#include "Sequence.h"

using namespace hrfm::display;

namespace hrfm{ namespace signage{ namespace display{
    
    class Sequence;
    
    class Transition : public hrfm::events::EventDispatcher{
        
    public:
        
        Transition();
        Transition( int width, int height );
        Transition( ci::Vec2i size );
        ~Transition();
        
        virtual void setSize( float width, float height );
        
        virtual void prepare();
        virtual void setCurrent( DisplayNode * content );
        virtual void setNext( DisplayNode * content );
        virtual void start( float time );
        
        virtual bool running();
        
        virtual void update();
        
        virtual ci::gl::Texture getTexture();
        
    protected:
        
        ci::gl::Fbo * _fbo;
        ci::gl::Fbo * _currentFbo;
        ci::gl::Fbo * _nextFbo;
        
        DisplayNode * _current;
        DisplayNode * _next;
        
        float _time;
        float _startedAt;
        bool  _running;
        
    };
    
}}}