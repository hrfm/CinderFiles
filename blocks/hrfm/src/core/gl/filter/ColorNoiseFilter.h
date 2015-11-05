#pragma once
#include "FilterBase.h"
namespace hrfm { namespace gl{ namespace filter{
    class ColorNoiseFilter : public FilterBase{
    public:
        ColorNoiseFilter():FilterBase(fs::path("ColorNoiseFilter.glsl")){};
    };
}}}