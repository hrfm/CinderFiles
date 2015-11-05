#pragma once

#include "FilterBase.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    class SciFiFilter : public FilterBase{
    public:
        SciFiFilter():FilterBase(fs::path("SciFiFilter.glsl")){};
    };
}}}