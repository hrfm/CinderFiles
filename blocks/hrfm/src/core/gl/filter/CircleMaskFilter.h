#pragma once
#include "FilterBase.h"
namespace hrfm { namespace gl{ namespace filter{
    class CircleMaskFilter : public FilterBase{
    public:
        CircleMaskFilter():FilterBase(ci::fs::path("CircleMaskFilter.glsl")){};
    };
}}}