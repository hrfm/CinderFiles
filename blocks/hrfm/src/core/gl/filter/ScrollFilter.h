#pragma once
#include "FilterBase.h"
namespace hrfm { namespace gl{ namespace filter{
    class ScrollFilter : public FilterBase{
    public:
        ScrollFilter( ci::ivec2 size = ci::ivec2(256,256) ):FilterBase(ci::fs::path("ScrollFilter.glsl"),size){};
        void setUniform( float scrollX = 0, float scrollY = 0 ){
            _scrollX = scrollX;
            _scrollY = scrollY;
        }
    protected:
        virtual void prepare(){
            mShader->uniform( "scrollX", _scrollX );
            mShader->uniform( "scrollY", _scrollY );
        }
        float _scrollX;
        float _scrollY;
    };
}}}