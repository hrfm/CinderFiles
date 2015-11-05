#pragma once

#include "FilterBase.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    class ScanLineFilter : public FilterBase{
    public:
        ScanLineFilter():FilterBase(fs::path("ScanLineFilter.glsl")){};
    };
}}}