#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class AnalogNoiseFilter : public FilterBase{
    public:
        AnalogNoiseFilter();
        float value;
    protected:
        DataSourceRef getFragmentShader();
        void prepare();
        void clear();
    };
    
}}}