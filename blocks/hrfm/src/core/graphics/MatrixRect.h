#pragma once

#include "cinder/app/App.h"

#include "hrfm.h"

namespace hrfm{ namespace graphics{
    
    class MatrixRect:public hrfm::display::DisplayNode{
        
    public:
        
        MatrixRect( ci::ivec2 size = ivec2( 8,10 ), vec2 spacing = vec2(1,1) ):hrfm::display::DisplayNode(){
            _init( size, spacing );
        }
        
        MatrixRect( hrfm::structure::ValueMatrix<ColorA> * matrix, vec2 spacing = vec2(1,1) ):hrfm::display::DisplayNode(){
            _init( matrix, spacing );
        }
        
    protected:
        
        void _init( ci::ivec2 size, vec2 spacing );
        void _init( hrfm::structure::ValueMatrix<ColorA> * matrix, vec2 spacing );
        virtual void _draw();
        
        vec2 _spacing;
        
        hrfm::structure::ValueMatrix<ColorA> * _mtx;
        
    };
    
}}