#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "SiFboFactory.h"

using namespace ci;
using namespace ci::gl;

namespace hrfm { namespace matrix{
    
    class MatrixBase{
        
    public:
        
        MatrixBase();
        
        void setup( ivec2 segments );
        
        int numCols();
        int numRows();
        
    private:
        
        ci::gl::FboRef _matrixFbo;
        
    };
    
}}