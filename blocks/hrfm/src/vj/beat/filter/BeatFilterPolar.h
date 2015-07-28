#pragma once

#include "BeatFilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm{ namespace vj{
    
    class BeatFilterPolar : public hrfm::vj::BeatFilterBase{
        
    public:
        
        BeatFilterPolar(){
            hrfm::vj::BeatFilterBase();
            //_segments = 8.0;
        };
        
        virtual void update(){
            //_segments = (float)randInt( 1, 16 );
        }
        
    protected:
        
        virtual DataSourceRef getFragmentShader(){
            return DataLoader::load("BeatFilterPolar.glsl");
        };
        
        virtual void prepare(){
            //mShader.uniform("segments",_segments);
        }
        
    private:
        
    };
    
}}