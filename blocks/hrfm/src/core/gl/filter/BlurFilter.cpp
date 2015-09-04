#include "BlurFilter.h"

namespace hrfm { namespace gl{ namespace filter{
    
    void BlurFilter::setUniform( Vec2f sample_offset, float attenuation ){
        mSampleOffset = sample_offset;
        mAttenuation  = attenuation;
    }
    
    //! protected:
    
    void BlurFilter::prepare(){
        mShader.uniform( "sample_offset", mSampleOffset );
        mShader.uniform( "attenuation"  , mAttenuation );
    }
    
    void BlurFilter::clear(){}
    
}}}