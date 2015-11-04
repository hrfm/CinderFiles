#include "SplitFilter.h"

namespace hrfm { namespace gl{ namespace filter{
    
    void SplitFilter::prepare(){
        mShader->uniform( "cols", this->cols );
        mShader->uniform( "rows", this->rows );
        mShader->uniform( "cliprect", this->clipRect );
    }
    
    void SplitFilter::clear(){}
    
}}}