#pragma once
#include "FilterBase.h"
using namespace hrfm::utils;
namespace hrfm { namespace gl{ namespace filter{
    class ThresholdFilter : public FilterBase{
    public:
        ThresholdFilter( ci::ivec2 size = ci::ivec2(256,256) ):FilterBase(fs::path("ThresholdFilter.glsl"),size){};
    };
}}}