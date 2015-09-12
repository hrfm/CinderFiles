#pragma once
#include "FilterBase.h"
namespace hrfm { namespace gl{ namespace filter{
    class AnalogNoiseFilter : public FilterBase{
    public:
        AnalogNoiseFilter():FilterBase("AnalogNoiseFilter.glsl"){};
    };
}}}