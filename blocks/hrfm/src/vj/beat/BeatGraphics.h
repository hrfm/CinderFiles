#pragma once

#include "hrfm.h"
#include "BeatContentBase.h"

#include "BeatSquare.h"
#include "BeatFilterSplit.h"

namespace hrfm{ namespace vj{
    
    class BeatGraphics : public hrfm::display::DisplayNode{
        
    public:
        
        BeatGraphics();
        virtual void setSize( int w, int h );
        virtual void addContent( BeatContentBase * content );
        virtual void addFilter( BeatFilterBase * filter );
        virtual void addFilter( BeatFilterBase * filter, int index );
        
    protected:
        
        virtual void _update();
        virtual void _draw();
        
    private:
        
        void _onBPM( hrfm::events::Event * event = NULL );
        
        hrfm::gl::ExFbo * _fbo;
        hrfm::vj::SiBPM * _bpm;
        
        vector<hrfm::vj::BeatContentBase*> _contentVector;
        hrfm::vj::BeatContentBase * _content;
        
        vector<vector<hrfm::vj::BeatFilterBase*>*> _filterStock;
        vector<hrfm::vj::BeatFilterBase*> _activeFilters;
        
    };

}}