#include "ColorNoiseFilter.h"

namespace hrfm { namespace gl{ namespace filter{
    
    ColorNoiseFilter::ColorNoiseFilter(){
        FilterBase();
        value = 0;
    }
    
    //! protected:
    
    DataSourceRef ColorNoiseFilter::getFragmentShader(){
        return DataLoader::load("ColorNoiseFilter.glsl");
    }
    
    void ColorNoiseFilter::prepare(){
        mShader.uniform( "value", value );
    }
    
    void ColorNoiseFilter::clear(){}
    
}}}