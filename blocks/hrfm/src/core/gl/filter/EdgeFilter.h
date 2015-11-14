#pragma once
#include "FilterBase.h"
namespace hrfm { namespace gl{ namespace filter{
    class EdgeFilter : public FilterBase{
    public:
        EdgeFilter( ci::ivec2 size = ivec2(256,256) ):FilterBase(fs::path("EdgeFilter.glsl"),size){};
    };
}}}