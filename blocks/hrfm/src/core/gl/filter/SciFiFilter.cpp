#include "SciFiFilter.h"

namespace hrfm { namespace gl{ namespace filter{
    
    //! public:
    
    SciFiFilter::SciFiFilter(){
        FilterBase();
    }
    
    //! protected:
    
    DataSourceRef SciFiFilter::getFragmentShader(){
        return DataLoader::load("SciFiFilter.glsl");
    }
    
}}}