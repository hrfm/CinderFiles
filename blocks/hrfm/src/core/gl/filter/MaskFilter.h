#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class MaskFilter : public FilterBase{
        
    public:
        
        MaskFilter( ci::gl::Texture * tex = NULL ):FilterBase("MaskFilter.glsl"){
            strength = 1.0;
            if( tex != NULL ){
                setTexture( tex );
            }
        };
        
        void setTexture( ci::gl::Texture * tex ){
            _tex = tex;
        };
        
        float strength;
        
    protected:
        
        virtual void prepare(){
            _tex->bind(1);
            mShader.uniform( "maskTex", 1 );
            mShader.uniform( "strength", strength );
        }
        
        virtual void clear(){
            _tex->unbind();
        }
        
        ci::gl::Texture * _tex;
        
    };
    
}}}