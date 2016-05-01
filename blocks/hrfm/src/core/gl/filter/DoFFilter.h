#pragma once
#include "FilterBase.h"
using namespace hrfm::utils;
namespace hrfm { namespace gl{ namespace filter{
    
    class DoFFilter : public FilterBase{
        
    public:
        DoFFilter( ci::gl::TextureRef tex = NULL ):FilterBase(ci::fs::path("DoFFilter.glsl")){
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
            mShader->uniform( "depthTex", 1 );
        }
        
        virtual void clear(){
            FilterBase::clear();
            _tex->unbind();
        }
        
        ci::gl::TextureRef _tex;
        
    };
    
}}}