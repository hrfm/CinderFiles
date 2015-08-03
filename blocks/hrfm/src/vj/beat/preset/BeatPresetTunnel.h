#pragma once

#include "BeatPresetBase.h"
#include "BeatFilterSplit.h"
#include "BeatFilterTransTunnel.h"

namespace hrfm{ namespace vj{
    
    class BeatPresetTunnel : public BeatPresetBase{
        
    public :
        
        BeatPresetTunnel( float segmentX = 3.0, float segmentY = 8.0 ):BeatPresetBase(){
            
            BeatPresetBase();
            
            hrfm::vj::BeatFilterSplit * split = new hrfm::vj::BeatFilterSplit();
            split->setup( Vec2i(1024,1024) );
            
            hrfm::vj::BeatFilterTransTunnel * tunnel = new hrfm::vj::BeatFilterTransTunnel( segmentX, segmentY );
            tunnel->setup( Vec2i(1024,1024) );
            
            addFilter(split);
            addFilter(split);
            addFilter(tunnel);
            
        };
        
    protected:
        
    };
    
    
}}