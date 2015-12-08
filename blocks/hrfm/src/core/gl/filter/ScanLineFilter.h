#pragma once

#include "FilterBase.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    class ScanLineFilter : public FilterBase{
    public:
        ScanLineFilter( ci::ivec2 size = ci::ivec2(256,256) ):FilterBase(ci::fs::path("ScanLineFilter.glsl"),size){};
    };
}}}