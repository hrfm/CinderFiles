#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class DigitalNoiseFilter : public FilterBase{
    public:
        DigitalNoiseFilter():FilterBase(fs::path("DigitalNoiseFilter.glsl")){
            isVertical = 0;
        };
        int isVertical;
    protected:
        void prepare();
        void clear();
    };
    
}}}