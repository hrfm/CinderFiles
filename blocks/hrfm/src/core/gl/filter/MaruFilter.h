#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm{ namespace gl{ namespace filter{
    
    class MaruFilter:public hrfm::gl::FilterBase{
    
    public:
        
        MaruFilter( ci::vec2 mtx = ci::vec2( 320, 180 ) ):FilterBase(ci::fs::path("MaruFilter.glsl")){
            _mtx = mtx;
        };
        
        void setMaskTexture( ci::gl::Texture * tex ){
            _tex = tex;
        }
        
        void enableMaskTexture( bool flag ){
            _enableMaskTexture = flag && _tex != NULL;
        }
        
    protected:
        
        virtual void prepare(){
            FilterBase::prepare();
            mShader->uniform("matrix", _mtx / floor(10*_strength) );
            if( _enableMaskTexture ){
                _tex->bind(1);
                mShader->uniform("useTexture", 1 );
                mShader->uniform("maskTex", 1 );
            }else{
                mShader->uniform("useTexture", 0 );
            }
        }
        
        virtual void clear(){
            if( _enableMaskTexture ){
                _tex->unbind();
            }
        }
        
    private:
        
        ci::vec2 _mtx;
        
        ci::gl::Texture * _tex = NULL;
        bool _enableMaskTexture;
        
    };

}}}