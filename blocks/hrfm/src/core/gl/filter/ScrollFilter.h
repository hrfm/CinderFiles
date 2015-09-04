#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class ScrollFilter : public FilterBase{
        
    public:
        
        ScrollFilter():FilterBase("ScrollFilter.glsl"){};
        
        void setUniform( float scrollX = 0, float scrollY = 0 );
        
    protected:
        
        virtual void prepare();
        
        float _scrollX;
        float _scrollY;
        
    };
    
}}}