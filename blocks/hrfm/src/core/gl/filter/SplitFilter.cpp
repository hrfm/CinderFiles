#include "SplitFilter.h"

namespace hrfm { namespace gl{ namespace filter{
    
    DataSourceRef SplitFilter::getFragmentShader(){
        return DataLoader::load("SplitFilter.glsl");
    }
    
    void SplitFilter::prepare(){
        cout << cols << ":" << rows << endl;
        mShader.uniform( "cols", this->cols );
        mShader.uniform( "rows", this->rows );
    }
    
    void SplitFilter::clear(){}
    
}}}