#pragma once
#include "FilterBase.h"
using namespace hrfm::utils;
namespace hrfm { namespace gl{ namespace filter{
    class ThresholdFilter : public FilterBase{
    public:
        ThresholdFilter():FilterBase(fs::path("ThresholdFilter.glsl")){};
    };
}}}