#pragma once
#include "FilterBase.h"
using namespace hrfm::utils;
namespace hrfm { namespace gl{ namespace filter{
    
    class MaskFilter : public FilterBase{
        
    public:
        MaskFilter( ci::gl::TextureRef tex = NULL ):FilterBase(ci::fs::path("MaskFilter.glsl")){
            if( tex != NULL ){
                setTexture( tex );
            }
        };
        void setTexture( ci::gl::TextureRef tex ){
            _tex = tex;
        };
    protected:
        
        virtual void prepare(){
            FilterBase::prepare();
            _tex->bind(1);
            mShader->uniform( "maskTex", 1 );
        }
        
        virtual void clear(){
            FilterBase::clear();
            _tex->unbind();
        }
        
        ci::gl::TextureRef _tex;
        
    };
    
}}}