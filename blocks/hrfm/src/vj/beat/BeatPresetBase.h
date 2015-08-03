#pragma once

#include "hrfm.h"
#include "BeatContentBase.h"
#include "BeatFilterBase.h"

namespace hrfm{ namespace vj{
    
    class BeatPresetBase{
        
    public :
        
        BeatPresetBase();
        
        virtual BeatPresetBase * addContent( BeatContentBase * content );
        virtual BeatPresetBase * addFilter( BeatFilterBase * filter );
        virtual BeatPresetBase * addFilter( BeatFilterBase * filter, int index );
        
        BeatContentBase * getContent();
        vector<BeatFilterBase*> * getActiveFilters();
        
        virtual void change( int width, int height );
        
    protected:
        
        vector<BeatContentBase*> _contentVector;
        BeatContentBase * _content;
        
        vector<vector<BeatFilterBase*>*> _filterStock;
        vector<BeatFilterBase*> * _activeFilters;
        
    };
    
    
}}