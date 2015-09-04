#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class AnalogNoiseFilter : public FilterBase{
    public:
        AnalogNoiseFilter():FilterBase("AnalogNoiseFilter.glsl"){
            value = 0;
        };
        float value;
    protected:
        virtual void prepare();
        virtual void clear();
    };
    
}}}