#pragma once

#include "hrfm.h"
#include "OutFilter.h"

namespace hrfm{ namespace vj{
    
    class BeatGraphics : public hrfm::display::DisplayNode{
        
    public:
        
        BeatGraphics(){
            
            hrfm::display::FboDisplayNode();
            
            _bpm = &hrfm::utils::SiBPM::getInstance();
            _bpm->addEventListener("bpm",this,&BeatGraphics::_onBPM);
            
            _fbo = new hrfm::gl::ExFbo(128,128);
            
            OutFilter * f = new OutFilter();
            f->setup( Vec2i(1024,1024) );
            _filters.push_back(f);
            
        }
        
        virtual void setSize( int w, int h );
        
    protected:
        
        virtual void _update();
        virtual void _draw();
        
    private:
        
        void _onBPM( hrfm::events::Event * event );
        
        hrfm::gl::ExFbo * _fbo;
        hrfm::utils::SiBPM * _bpm;
        
        vector<hrfm::gl::FilterBase*> _filters;
        
    };

}}