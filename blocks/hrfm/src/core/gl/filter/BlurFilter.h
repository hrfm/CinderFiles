#pragma once

#include "FilterBase.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class BlurFilter : public FilterBase{
    public:
        BlurFilter( ci::vec2 size = ci::vec2(256,256) ):FilterBase(ci::fs::path("BlurFilter.glsl"),size){};
        void setUniform( vec2 sample_offset, float attenuation ){
            mSampleOffset = sample_offset;
            mAttenuation  = attenuation;
        }
    protected:
        void prepare(){
            FilterBase::prepare();
            mShader->uniform( "sample_offset", mSampleOffset );
            mShader->uniform( "attenuation"  , mAttenuation );
        }
        void clear(){
            FilterBase::clear();
        }
        vec2 mSampleOffset;
        float mAttenuation;
    };
    
}}}