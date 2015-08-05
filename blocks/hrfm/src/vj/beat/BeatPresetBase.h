#pragma once

#include "hrfm.h"
#include "BeatContentBase.h"
#include "BeatFilterBase.h"

namespace hrfm{ namespace vj{
    
    class BeatPresetBase{
        
    public :
        
        BeatPresetBase();
        
        virtual BeatPresetBase * addContent( BeatContentBase * content );
        virtual BeatPresetBase * addFilter( hrfm::gl::FilterBase * filter );
        virtual BeatPresetBase * addFilter( hrfm::gl::FilterBase * filter, int index );
        
        BeatContentBase * getContent();
        vector<hrfm::gl::FilterBase*> * getActiveFilters();
        
        virtual void change( int width, int height );
        
    protected:
        
        vector<BeatContentBase*> _contentVector;
        BeatContentBase * _content;
        
        vector<vector<hrfm::gl::FilterBase*>*> _filterStock;
        vector<hrfm::gl::FilterBase*> * _activeFilters;
        
    };
    
    
}}