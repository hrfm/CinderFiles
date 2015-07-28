#pragma once

#include "BeatFilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm{ namespace vj{

    class BeatFilterSplit : public hrfm::vj::BeatFilterBase{
        
    public:
        
        BeatFilterSplit(){
            hrfm::vj::BeatFilterBase();
            _segments = 8.0;
        };
        
        virtual void update(){
            _segments = (float)randInt( 1, 8 );
        }
        
    protected:
        
        virtual DataSourceRef getFragmentShader(){
            return DataLoader::load("BeatFilterSplit.glsl");
        };
        
        virtual void prepare(){
            mShader.uniform("segments",_segments);
        }
    
    private:
        
        float _segments;
        
    };

}}