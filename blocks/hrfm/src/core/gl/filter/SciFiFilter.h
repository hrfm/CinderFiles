#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class SciFiFilter : public FilterBase{
        
    public:
        
        SciFiFilter():FilterBase("SciFiFilter.glsl"){};
        
    protected:
        
    };
    
}}}