#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class SplitFilter : public FilterBase{
    public:
        SplitFilter( float cols = 1.0, float rows = 1.0, Vec4f clipRect = Vec4f(0.0, 0.0, 1.0, 1.0) ):FilterBase("SplitFilter.glsl"){
            this->cols     = cols;
            this->rows     = rows;
            this->clipRect = clipRect;
        };
        float cols;
        float rows;
        Vec4f clipRect;
    protected:
        virtual void prepare();
        virtual void clear();
    };
    
}}}