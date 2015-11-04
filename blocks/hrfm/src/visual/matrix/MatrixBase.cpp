#include "MatrixBase.h"

using namespace hrfm::gl;

namespace hrfm { namespace matrix{

    MatrixBase::MatrixBase(){
        
    }
    
    void MatrixBase::setup( ivec2 segments ){
        _matrixFbo = SiFboFactory::getInstance().create( segments.x, segments.y );
    }
    
    int MatrixBase::numCols(){
        return _matrixFbo->getWidth();
    }
    
    int MatrixBase::numRows(){
        return _matrixFbo->getHeight();
    }
    
}}