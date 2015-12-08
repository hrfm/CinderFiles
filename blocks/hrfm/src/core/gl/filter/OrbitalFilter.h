#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class OrbitalFilter : public FilterBase{
        
    public:
        
        OrbitalFilter( ci::ivec2 size = ci::ivec2(256,256) ):FilterBase(ci::fs::path("OrbitalFilter.glsl"),size){
            r_scale = 1.0;
            r_min   = 0.0;
            r_max   = 1.0;
            _speedTex = ci::gl::Texture2d::create( 1, 1 );
        };
        
        OrbitalFilter( ci::gl::TextureRef tex_speed, ci::ivec2 size = ci::ivec2(256,256) ):FilterBase(ci::fs::path("OrbitalFilter.glsl"),size){
            r_scale = 1.0;
            r_min   = 0.0;
            r_max   = 1.0;
            _speedTex = tex_speed;
        }
        
        void setSpeedTex( ci::gl::TextureRef tex_speed ){
            _speedTex = tex_speed;
        }
        
        float r_min;
        float r_max;
        float r_scale;
        
    protected:
        
        void prepare(){
            _speedTex->bind(1);
            mShader->uniform( "tex_speed", 1 );
            mShader->uniform( "r_scale", r_scale );
            mShader->uniform( "r_min", r_min );
            mShader->uniform( "r_max", r_max );
        }
        void clear(){
            _speedTex->unbind();
        }
        
    private:
        
        ci::gl::TextureRef _speedTex;
        
    };
    
}}}