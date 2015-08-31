#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class SplitFilter : public FilterBase{
    public:
        SplitFilter( float cols = 1, float rows = 1 ):FilterBase(){
            this->cols = cols;
            this->rows = rows;
        };
        float cols;
        float rows;
    protected:
        DataSourceRef getFragmentShader();
        void prepare();
        void clear();
    };
    
}}}