#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class CircleMaskFilter : public FilterBase{
        
    public:
        
        CircleMaskFilter();
        
    protected:
        
        DataSourceRef getFragmentShader();
        
    };
    
}}}