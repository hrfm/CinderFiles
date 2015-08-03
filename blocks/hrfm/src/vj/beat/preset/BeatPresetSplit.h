#pragma once

#include "BeatPresetBase.h"
#include "BeatFilterSplit.h"

namespace hrfm{ namespace vj{
    
    class BeatPresetSplit : public BeatPresetBase{
        
    public :
        
        BeatPresetSplit():BeatPresetBase(){
            
            BeatPresetBase();
            
            hrfm::vj::BeatFilterBase * blank = new hrfm::vj::BeatFilterBase();
            blank->setup( Vec2i(1024,1024) );
            
            hrfm::vj::BeatFilterSplit * split = new hrfm::vj::BeatFilterSplit();
            split->setup( Vec2i(1024,1024) );
            
            hrfm::vj::BeatFilterSplit * split2 = new hrfm::vj::BeatFilterSplit();
            split2->setup( Vec2i(1024,1024) );
            {
                vector<SplitSetting*> * seq = new vector<SplitSetting*>();
                for( int i=0;i<1;i++ ){
                    SplitSetting * setting = new SplitSetting();
                    setting->segments = Vec2f( 2, 2 );
                    setting->rotate   = i;
                    seq->push_back(setting);
                }
                split2->setSequence(seq);
            }
            
            addFilter(split2,0);
            addFilter(blank,1);
            addFilter(split2,1);
            addFilter(blank,2);
            addFilter(split2,2);
            
        };
        
    protected:
        
    };
    
    
}}