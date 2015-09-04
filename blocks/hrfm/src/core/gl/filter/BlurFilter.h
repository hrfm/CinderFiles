#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class BlurFilter : public FilterBase{
    public:
        BlurFilter():FilterBase("BlurFilter.glsl"){
            
        };
        void setUniform( Vec2f sample_offset, float attenuation );
    protected:
        DataSourceRef getFragmentShader();
        void prepare();
        void clear();
        Vec2f mSampleOffset;
        float mAttenuation;
    };
    
}}}