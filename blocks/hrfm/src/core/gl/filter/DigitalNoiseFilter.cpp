#include "DigitalNoiseFilter.h"

namespace hrfm { namespace gl{ namespace filter{
    
    //! protected:
    
    void DigitalNoiseFilter::prepare(){
        FilterBase::prepare();
        mShader.uniform( "vertical", isVertical );
    }
    
    void DigitalNoiseFilter::clear(){
        FilterBase::clear();
    }
    
}}}