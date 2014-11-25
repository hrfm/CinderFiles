#pragma once

#include "FontMatrix.h"
#include "TransitionShaderBase.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace hrfm::text;
using namespace hrfm::utils;

namespace hrfm { namespace gl{
    
    class MatrixTransition : public TransitionShaderBase{
        
    public:
        
        MatrixTransition();
        
        virtual void prepare();
        
        virtual void bindTexture( const ci::gl::Texture & current, const ci::gl::Texture & next );
        
        virtual void affect( float progress );
        
    protected:
        
        FontMatrix _matrix;
        
    };

}}