#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class ScrollFilter : public FilterBase{
        
    public:
        
        ScrollFilter();
        
        void setUniform( float scrollX = 0, float scrollY = 0 );
        
    protected:
        
        DataSourceRef getFragmentShader();
        
        void prepare();
        
        float _scrollX;
        float _scrollY;
        
    };
    
}}}