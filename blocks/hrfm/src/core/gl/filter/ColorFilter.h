#pragma once
#include "FilterBase.h"
namespace hrfm { namespace gl{ namespace filter{
    
    class ColorFilter : public FilterBase{
    
    public:
        
        ColorFilter():FilterBase("ColorFilter.glsl"){
            color = vec3(1.0f,1.0f,1.0f);
        };
        
        ci::vec3 color;
        
    protected:
        
        virtual void prepare(){
            FilterBase::prepare();
            mShader->uniform("color", color );
        }
        
    };
    
}}}