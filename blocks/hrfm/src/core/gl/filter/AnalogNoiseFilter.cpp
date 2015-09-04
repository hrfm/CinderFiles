#include "AnalogNoiseFilter.h"

namespace hrfm { namespace gl{ namespace filter{
    
    //! protected:
    
    void AnalogNoiseFilter::prepare(){
        mShader.uniform( "value", value );
    }
    
    void AnalogNoiseFilter::clear(){}
    
}}}