#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    
    class OrbitalFilter : public FilterBase{
        
    public:
        
        OrbitalFilter():FilterBase(fs::path("OrbitalFilter.glsl")){
            r_scale = 1.0;
            r_min   = 0.0;
            r_max   = 1.0;
            _speedTex = ci::gl::Texture2d::create( 1, 1 );
        };
        
        OrbitalFilter( ci::gl::TextureRef tex_speed ):FilterBase(fs::path("OrbitalFilter.glsl")){
            r_scale = 1.0;
            r_min   = 0.0;
            r_max   = 1.0;
            _speedTex = tex_speed;
        }
        
        void setSpeedTex( ci::gl::TextureRef tex_speed );
        
        float r_min;
        float r_max;
        float r_scale;
        
    protected:
        
        void prepare();
        void clear();
        
    private:
        
        ci::gl::TextureRef _speedTex;
        
    };
    
}}}