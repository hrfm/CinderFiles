#include "ScrollFilter.h"

namespace hrfm { namespace gl{ namespace filter{
    
    ScrollFilter::ScrollFilter(){
        FilterBase();
    }
    void ScrollFilter::setUniform( float scrollX, float scrollY ){
        _scrollX = scrollX;
        _scrollY = scrollY;
    }
    
    DataSourceRef ScrollFilter::getFragmentShader(){
        return DataLoader::load("ScrollFilter.glsl");
    }
    
    void ScrollFilter::prepare(){
        mShader.uniform( "scrollX", _scrollX );
        mShader.uniform( "scrollY", _scrollY );
    }
    
}}}