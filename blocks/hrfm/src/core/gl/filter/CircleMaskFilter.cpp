#include "CircleMaskFilter.h"

namespace hrfm { namespace gl{ namespace filter{
    
    //! public:
    
    CircleMaskFilter::CircleMaskFilter(){
        FilterBase();
    }
    
    //! protected:
    
    DataSourceRef CircleMaskFilter::getFragmentShader(){
        return DataLoader::load("CircleMaskFilter.glsl");
    }
    
}}}