#pragma once
#include "FilterBase.h"
namespace hrfm { namespace gl{ namespace filter{
    class ScrollFilter : public FilterBase{
    public:
        ScrollFilter():FilterBase(fs::path("ScrollFilter.glsl")){};
        void setUniform( float scrollX = 0, float scrollY = 0 );
    protected:
        virtual void prepare();
        float _scrollX;
        float _scrollY;
    };
}}}