#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    class ColorNoiseFilter : public FilterBase{
    public:
        ColorNoiseFilter():FilterBase("ColorNoiseFilter.glsl"){
            value = 0.0;
        };
        float value;
    protected:
        virtual void prepare();
        virtual void clear();
    };
}}}