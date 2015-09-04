#include "DigitalNoiseFilter.h"

namespace hrfm { namespace gl{ namespace filter{
    
    //! protected:
    
    void DigitalNoiseFilter::prepare(){
        mShader.uniform( "value", value );
        mShader.uniform( "vertical", isVertical );
    }
    
    void DigitalNoiseFilter::clear(){}
    
}}}