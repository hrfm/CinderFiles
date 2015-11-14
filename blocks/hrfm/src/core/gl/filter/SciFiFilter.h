#pragma once

#include "FilterBase.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    class SciFiFilter : public FilterBase{
    public:
        SciFiFilter( ci::ivec2 size = ci::ivec2(256,256) ) : FilterBase( fs::path("SciFiFilter.glsl"), size ){};
    };
}}}