#pragma once

#include "cinder/app/App.h"
#include "Singleton.h"
#include <EventDispatcher.h>

#include "hrfm.h"
#include "hrfm.gl.h"

using namespace std;

namespace hrfm{ namespace vj{
    
    class VJGraphic : public hrfm::display::DisplayNode{
        
    public:
        
        VJGraphic();
        virtual void start();
        virtual void stop();
        virtual void setSize( int w, int h );
        
        virtual void addPreFilter( hrfm::gl::FilterBase * filter );
        virtual void addPostFilter( hrfm::gl::FilterBase * filter );
        
        virtual void update();
        virtual void draw( ci::ColorA * drawColor = NULL );
        
    protected:
        
        virtual void _onBPM( hrfm::events::Event * event = NULL );
        
        hrfm::gl::ExFbo * _fbo;
        hrfm::vj::SiBPM * _bpm;
        vector<hrfm::gl::FilterBase*> _preFilter;
        vector<hrfm::gl::FilterBase*> _postFilter;
        
    private:
        
    };
    
}}