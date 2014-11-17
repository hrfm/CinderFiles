#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class DigitalNoiseFilter : public FilterBase{
    public:
        DigitalNoiseFilter();
        float value;
        int isVertical;
    protected:
        DataSourceRef getFragmentShader();
        void prepare();
        void clear();
    };
    
}}}