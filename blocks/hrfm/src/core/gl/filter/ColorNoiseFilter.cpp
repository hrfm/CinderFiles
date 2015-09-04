#include "ColorNoiseFilter.h"

namespace hrfm { namespace gl{ namespace filter{
    
    //! protected:
    
    void ColorNoiseFilter::prepare(){
        mShader.uniform( "value", value );
    }
    
    void ColorNoiseFilter::clear(){}
    
}}}