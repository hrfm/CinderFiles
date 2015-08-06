#pragma once

#include "hrfm.h"
#include "VJGraphic.h"
#include "FilterBase.h"
#include "BeatPresetBase.h"

namespace hrfm{ namespace vj{
    
    class BeatGraphics : public VJGraphic{
        
    public:
        
        BeatGraphics();
        virtual void addPreset( BeatPresetBase * preset );
        virtual void displayPresetAt( int index );
        virtual void update();
        
    protected:
        
        virtual void _onBPM( hrfm::events::Event * event = NULL );
        
    private:
        
        BeatPresetBase * _preset = NULL;
        vector<BeatPresetBase*> _presetList;
        int _presetOrderPointer;
        vector<int> _presetOrder;
        
    };
    
}}