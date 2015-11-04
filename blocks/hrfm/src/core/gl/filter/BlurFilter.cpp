#include "BlurFilter.h"

namespace hrfm { namespace gl{ namespace filter{
    
    void BlurFilter::setUniform( vec2 sample_offset, float attenuation ){
        mSampleOffset = sample_offset;
        mAttenuation  = attenuation;
    }
    
    //! protected:
    
    void BlurFilter::prepare(){
        FilterBase::prepare();
        mShader.uniform( "sample_offset", mSampleOffset );
        mShader.uniform( "attenuation"  , mAttenuation );
    }
    
    void BlurFilter::clear(){
        FilterBase::clear();
    }
    
}}}