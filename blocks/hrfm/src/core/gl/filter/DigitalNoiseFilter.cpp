#include "DigitalNoiseFilter.h"

namespace hrfm { namespace gl{ namespace filter{
    
    DigitalNoiseFilter::DigitalNoiseFilter(){
        FilterBase();
        value      = 0;
        isVertical = 0;
    }
    
    //! protected:
    
    DataSourceRef DigitalNoiseFilter::getFragmentShader(){
        return DataLoader::load("DigitalNoiseFilter.glsl");
    }
    
    void DigitalNoiseFilter::prepare(){
        mShader.uniform( "value", value );
        mShader.uniform( "vertical", isVertical );
    }
    
    void DigitalNoiseFilter::clear(){}
    
}}}