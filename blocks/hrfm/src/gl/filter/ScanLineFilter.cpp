#include "ScanLineFilter.h"

namespace hrfm { namespace gl{ namespace filter{
    
    //! public:
    
    ScanLineFilter::ScanLineFilter(){
        FilterBase();
    }
    
    //! protected:
    
    DataSourceRef ScanLineFilter::getFragmentShader(){
        return DataLoader::load("ScanLineFilter.glsl");
    }
    
}}}