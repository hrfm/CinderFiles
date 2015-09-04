#include "ScrollFilter.h"

namespace hrfm { namespace gl{ namespace filter{
    
    void ScrollFilter::setUniform( float scrollX, float scrollY ){
        _scrollX = scrollX;
        _scrollY = scrollY;
    }
    
    void ScrollFilter::prepare(){
        mShader.uniform( "scrollX", _scrollX );
        mShader.uniform( "scrollY", _scrollY );
    }
    
}}}