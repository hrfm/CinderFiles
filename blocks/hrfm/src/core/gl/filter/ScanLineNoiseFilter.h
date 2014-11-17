#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class ScanLineNoiseFilter : public FilterBase{
        
    public:
        
        ScanLineNoiseFilter();
        
    protected:
        
        DataSourceRef getFragmentShader();
        
    };
    
}}}