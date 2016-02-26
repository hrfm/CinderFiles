#pragma once
#include "FilterBase.h"
namespace hrfm { namespace gl{ namespace filter{
    
    class ColorFilter : public FilterBase{
    
    public:
        
        ColorFilter( ci::ivec2 size = ci::ivec2(256,256) ):FilterBase(ci::fs::path("ColorFilter.glsl"),size){
            color   = vec3(1.0f,1.0f,1.0f);
            mode    = 0;
            inverse = false;
        };
        
        // フィルタの色指定.
        ci::vec3 color;
        // 1 にすると掛け算ではなく加算をする.
        int mode;
        // true にすると色を反転させる.
        bool inverse;
        // このテクスチャを指定するとその色を掛け合わせる.
        ci::gl::TextureRef colorTex = NULL;
        
    protected:
        
        virtual void prepare(){
            FilterBase::prepare();
            mShader->uniform( "color"  , color       );
            mShader->uniform( "mode"   , mode        );
            mShader->uniform( "inverse", inverse?1:0 );
            if( colorTex ){
                colorTex->bind();
                mShader->uniform( "useColorTex", 1 );
                mShader->uniform( "colorTex"   , 1 );
            }else{
                mShader->uniform( "useColorTex", 0 );
            }
        }
        
        virtual void clear(){
            FilterBase::clear();
            if( colorTex ){
                colorTex->unbind();
            }
        }
        
    };
    
}}}