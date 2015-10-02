#include "OrbitalFilter.h"

namespace hrfm { namespace gl{ namespace filter{
    
    //! public:
    
    void OrbitalFilter::setSpeedTex( ci::gl::Texture tex_speed ){
        _speedTex = tex_speed;
    }
    
    //! protected:
    
    void OrbitalFilter::prepare(){
        _speedTex.bind(1);
        mShader.uniform( "tex_speed", 1 );
        mShader.uniform( "r_scale", r_scale );
        mShader.uniform( "r_min", r_min );
        mShader.uniform( "r_max", r_max );
    }
    
    void OrbitalFilter::clear(){
        _speedTex.unbind();
    }
    
}}}