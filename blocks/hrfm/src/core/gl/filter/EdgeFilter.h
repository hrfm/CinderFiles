#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class EdgeFilter : public FilterBase{
    public:
        EdgeFilter();
    protected:
        DataSourceRef getFragmentShader();
        void prepare();
        void clear();
    };
    
}}}