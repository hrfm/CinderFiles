#pragma once

#include "hrfm.h"
#include "FilterBase.h"
#include "BeatPresetBase.h"

namespace hrfm{ namespace vj{
    
    class BeatGraphics : public hrfm::display::DisplayNode{
        
    public:
        
        BeatGraphics();
        virtual void setSize( int w, int h );
        virtual void addPreFilter( hrfm::gl::FilterBase * filter );
        virtual void addPostFilter( hrfm::gl::FilterBase * filter );
        virtual void addPreset( BeatPresetBase * preset );
        virtual void displayPresetAt( int index );
        
    protected:
        
        virtual void _update();
        virtual void _draw();
        
        vector<hrfm::gl::FilterBase*> _preFilter;
        vector<hrfm::gl::FilterBase*> _postFilter;
        
    private:
        
        void _onBPM( hrfm::events::Event * event = NULL );
        
        hrfm::gl::ExFbo * _fbo;
        hrfm::vj::SiBPM * _bpm;
        BeatPresetBase * _preset = NULL;
        
        vector<BeatPresetBase*> _presetList;
        
        int _presetOrderPointer;
        vector<int> _presetOrder;
        
    };

}}