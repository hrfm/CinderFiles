#pragma once

#include "BeatPresetBase.h"
#include "BeatFilterSplit.h"
#include "BeatFilterTransField.h"

namespace hrfm{ namespace vj{
    
    class BeatPresetFieldV : public BeatPresetBase{
        
    public :
        
        BeatPresetFieldV():BeatPresetBase(){
            
            BeatPresetBase();
            
            hrfm::vj::BeatFilterBase * blank = new hrfm::vj::BeatFilterBase();
            blank->setup( Vec2i(1024,1024) );
            
            hrfm::vj::BeatFilterSplit * split = new hrfm::vj::BeatFilterSplit();
            split->setup( Vec2i(1024,1024) );
            
            hrfm::vj::BeatFilterTransField * fieldV = new hrfm::vj::BeatFilterTransField(true);
            fieldV->setup( Vec2i(1024,1024) );
            
            addFilter(split,0);
            addFilter(blank,1);
            addFilter(split,1);
            addFilter(blank,2);
            addFilter(split,2);
            addFilter(fieldV);
            
        };
        
    protected:
        
    };
    
    
}}