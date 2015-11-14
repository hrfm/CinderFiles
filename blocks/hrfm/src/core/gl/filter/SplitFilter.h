#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class SplitFilter : public FilterBase{
    public:
        SplitFilter( ivec2 size = ivec2(256,256), float cols = 1.0, float rows = 1.0, vec4 clipRect = vec4(0.0, 0.0, 1.0, 1.0) ):FilterBase( fs::path("SplitFilter.glsl"), size ){
            this->cols     = cols;
            this->rows     = rows;
            this->clipRect = clipRect;
        };
        float cols;
        float rows;
        vec4 clipRect;
    protected:
        virtual void prepare(){
            mShader->uniform( "cols", this->cols );
            mShader->uniform( "rows", this->rows );
            mShader->uniform( "cliprect", this->clipRect );
        }
        virtual void clear(){};
    };
    
}}}