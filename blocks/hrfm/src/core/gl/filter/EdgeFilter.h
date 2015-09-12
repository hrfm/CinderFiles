#pragma once
#include "FilterBase.h"
namespace hrfm { namespace gl{ namespace filter{
    class EdgeFilter : public FilterBase{
    public:
        EdgeFilter():FilterBase("EdgeFilter.glsl"){};
    };
}}}