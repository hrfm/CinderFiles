#include "EdgeFilter.h"

namespace hrfm { namespace gl{ namespace filter{
    
    EdgeFilter::EdgeFilter(){
        FilterBase();
    }
    
    //! protected:
    
    DataSourceRef EdgeFilter::getFragmentShader(){
        return DataLoader::load("EdgeFilter.glsl");
    }
    
    void EdgeFilter::prepare(){}
    
    void EdgeFilter::clear(){}
    
}}}