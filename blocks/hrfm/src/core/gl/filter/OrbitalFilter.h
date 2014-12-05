#pragma once

#include "FilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm { namespace gl{ namespace filter{
    class OrbitalFilter : public FilterBase{
    public:
        
        OrbitalFilter();
        OrbitalFilter( ci::gl::Texture tex_speed );
        void setSpeedTex( ci::gl::Texture tex_speed );
        
        float r_min;
        float r_max;
        float r_scale;
        
    protected:
        DataSourceRef getFragmentShader();
        void prepare();
        void clear();
    private:
        ci::gl::Texture _speedTex;
    };
    
}}}