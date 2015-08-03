#pragma once

#include "hrfm.h"
#include "FilterBase.h"
#include "BeatPresetBase.h"

#include "BeatSquare.h"

#include "BeatFilterColor.h"
#include "BeatFilterSplit.h"
#include "BeatFilterTransField.h"
#include "BeatFilterTransTunnel.h"
#include "BeatFilterDot.h"
#include "BeatFilterScroll.h"

#include "BeatPresetFieldV.h"
#include "BeatPresetPolar.h"
#include "BeatPresetSplit.h"
#include "BeatPresetTunnel.h"

namespace hrfm{ namespace vj{
    
    class BeatGraphics : public hrfm::display::DisplayNode{
        
    public:
        
        BeatGraphics();
        virtual void setPresetByIndex( int index );
        virtual void setSize( int w, int h );
        
        virtual void addPreset( BeatPresetBase * preset );
        
        virtual void addPreFilter( hrfm::gl::FilterBase * filter );
        virtual void addPostFilter( hrfm::gl::FilterBase * filter );
        
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