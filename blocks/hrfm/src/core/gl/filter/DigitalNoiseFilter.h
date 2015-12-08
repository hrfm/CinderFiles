#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class DigitalNoiseFilter : public FilterBase{
    public:
        DigitalNoiseFilter( ci::ivec2 size = ci::ivec2(256,256) ):FilterBase(ci::fs::path("DigitalNoiseFilter.glsl"),size){
            isVertical = 0;
        };
        int isVertical;
    protected:
        void prepare(){
            FilterBase::prepare();
            mShader->uniform( "vertical", isVertical );
        }
        void clear(){
            FilterBase::clear();
        }
    };
    
}}}