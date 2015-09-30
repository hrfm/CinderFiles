#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class ThresholdFilter : public FilterBase{
        
    public:
        
        ThresholdFilter():FilterBase("ThresholdFilter.glsl"){};
        
    protected:
        
    };
    
}}}