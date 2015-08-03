#pragma once

#include "BeatPresetBase.h"
#include "BeatFilterSplit.h"
#include "BeatFilterTransPolar.h"

namespace hrfm{ namespace vj{
    
    class BeatPresetPolar : public BeatPresetBase{
        
    public :
        
        BeatPresetPolar( float segmentX = 2.0, float segmentY = 1.0 ):BeatPresetBase(){
            
            BeatPresetBase();
            
            hrfm::vj::BeatFilterBase * blank = new hrfm::vj::BeatFilterBase();
            blank->setup( Vec2i(1024,1024) );
            
            hrfm::vj::BeatFilterSplit * split = new hrfm::vj::BeatFilterSplit();
            split->setup( Vec2i(1024,1024) );
            {
                vector<SplitSetting*> * seq = new vector<SplitSetting*>();
                SplitSetting * setting = new SplitSetting();
                setting->segments = Vec2f( 2, 2 );
                setting->rotate   = 100;
                seq->push_back(setting);
                split->setSequence(seq);
            }
            
            hrfm::vj::BeatFilterTransPolar * polar = new hrfm::vj::BeatFilterTransPolar(segmentX, segmentY);
            polar->setup( Vec2i(1024,1024) );
            
            addFilter(split);
            addFilter(blank,1);
            addFilter(split,1);
            addFilter(polar);
            
        };
        
    protected:
        
    };
    
    
}}