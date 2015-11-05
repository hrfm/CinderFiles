#pragma once

#include "FilterBase.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class BlurFilter : public FilterBase{
    public:
        BlurFilter():FilterBase(fs::path("BlurFilter.glsl")){};
        void setUniform( vec2 sample_offset, float attenuation );
    protected:
        void prepare();
        void clear();
        vec2 mSampleOffset;
        float mAttenuation;
    };
    
}}}