#include "ScanLineNoiseFilter.h"

namespace hrfm { namespace gl{ namespace filter{
    
    //! public:
    
    ScanLineNoiseFilter::ScanLineNoiseFilter(){
        FilterBase();
    }
    
    //! protected:
    
    DataSourceRef ScanLineNoiseFilter::getFragmentShader(){
        return DataLoader::load("ScanLineNoiseFilter.glsl");
    }
    
}}}