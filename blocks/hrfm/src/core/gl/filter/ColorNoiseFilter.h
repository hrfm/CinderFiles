#pragma once
#include "FilterBase.h"
namespace hrfm { namespace gl{ namespace filter{
    class ColorNoiseFilter : public FilterBase{
    public:
        ColorNoiseFilter():FilterBase("ColorNoiseFilter.glsl"){};
    };
}}}