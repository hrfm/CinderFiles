#include "AnalogNoiseFilter.h"

namespace hrfm { namespace gl{ namespace filter{
    
    AnalogNoiseFilter::AnalogNoiseFilter(){
        FilterBase();
        value      = 0;
    }
    
    //! protected:
    
    DataSourceRef AnalogNoiseFilter::getFragmentShader(){
        return DataLoader::load("AnalogNoiseFilter.glsl");
    }
    
    void AnalogNoiseFilter::prepare(){
        mShader.uniform( "value", value );
    }
    
    void AnalogNoiseFilter::clear(){}
    
}}}