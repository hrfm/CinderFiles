#include "OrbitalFilter.h"

namespace hrfm { namespace gl{ namespace filter{
    
    //! public:
    
    OrbitalFilter::OrbitalFilter(){
        
        FilterBase();
        
        r_scale = 1.0;
        r_min   = 0.0;
        r_max   = 1.0;
        
        _speedTex = ci::gl::Texture( 1, 1 );
        
    }
    
    OrbitalFilter::OrbitalFilter( ci::gl::Texture tex_speed ){
        FilterBase();
        _speedTex = tex_speed;
    }
    
    void OrbitalFilter::setSpeedTex( ci::gl::Texture tex_speed ){
        _speedTex = tex_speed;
    }
    
    //! protected:
    
    DataSourceRef OrbitalFilter::getFragmentShader(){
        return DataLoader::load("OrbitalFilter.glsl");
    }
    
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