#pragma once
#include "FilterBase.h"
namespace hrfm { namespace gl{ namespace filter{
    class ColorNoiseFilter : public FilterBase{
    public:
        ColorNoiseFilter( ci::ivec2 size = ci::ivec2(256,256) ):FilterBase(ci::fs::path("ColorNoiseFilter.glsl"),size){
            direction = vec2(1.0,0.0);
        };
        vec2 direction;
    protected:
        virtual void prepare(){
            FilterBase::prepare();
            mShader->uniform( "direction", direction);
        }
    };
}}}