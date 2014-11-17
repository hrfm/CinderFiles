#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class ScanLineFilter : public FilterBase{
        
    public:
        
        ScanLineFilter();
        
    protected:
        
        DataSourceRef getFragmentShader();
        
    };
    
}}}