#pragma once

#include "FilterBase.h"

namespace hrfm { namespace gl{ namespace filter{
    class ScanLineNoiseFilter : public FilterBase{
    public:
        ScanLineNoiseFilter():FilterBase(fs::path("ScanLineNoiseFilter.glsl")){}
    };
}}}