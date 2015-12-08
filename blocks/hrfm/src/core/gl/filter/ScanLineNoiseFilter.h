#pragma once

#include "FilterBase.h"

namespace hrfm { namespace gl{ namespace filter{
    class ScanLineNoiseFilter : public FilterBase{
    public:
        ScanLineNoiseFilter( ci::ivec2 size = ci::ivec2(256,256) ):FilterBase(ci::fs::path("ScanLineNoiseFilter.glsl"),size){}
    };
}}}