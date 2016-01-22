#pragma once
#include "FilterBase.h"
namespace hrfm { namespace gl{ namespace filter{
    class ColorFilter : public FilterBase{
    public:
        ColorFilter( ci::ivec2 size = ci::ivec2(256,256) ):FilterBase(ci::fs::path("ColorFilter.glsl"),size){
            color = vec3(1.0f,1.0f,1.0f);
        };
        ci::vec3 color;
        int mode = 0;
        bool inverse = false;
    protected:
        virtual void prepare(){
            FilterBase::prepare();
            mShader->uniform("color", color );
            mShader->uniform("mode", mode );
            mShader->uniform("inverse", inverse?1:0 );
        }
    };
}}}