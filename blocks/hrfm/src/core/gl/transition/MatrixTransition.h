#pragma once

#include "FontMatrix.h"
#include "TransitionShaderBase.h"

using namespace ci;
using namespace std;
using namespace hrfm::text;
using namespace hrfm::utils;

namespace hrfm { namespace gl{
    
    class MatrixTransition : public TransitionShaderBase{
        
    public:
        
        MatrixTransition();
        
        virtual void prepare();
        
        virtual void bindTexture( const ci::gl::TextureRef current, const ci::gl::TextureRef next );
        
        virtual void affect( float progress );
        
    protected:
        
        FontMatrix _matrix;
        
        double _beforeTime;
        
    };

}}