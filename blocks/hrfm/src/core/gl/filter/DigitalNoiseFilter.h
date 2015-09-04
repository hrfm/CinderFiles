#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class DigitalNoiseFilter : public FilterBase{
    public:
        DigitalNoiseFilter():FilterBase("DigitalNoiseFilter.glsl"){
            value      = 0;
            isVertical = 0;
        };
        float value;
        int isVertical;
    protected:
        void prepare();
        void clear();
    };
    
}}}