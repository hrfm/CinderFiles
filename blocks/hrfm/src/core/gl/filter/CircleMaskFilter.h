#pragma once
#include "FilterBase.h"
namespace hrfm { namespace gl{ namespace filter{
    class CircleMaskFilter : public FilterBase{
    public:
        CircleMaskFilter():FilterBase(fs::path("CircleMaskFilter.glsl")){};
    };
}}}