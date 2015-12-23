#include "MatrixRect.h"

namespace hrfm{ namespace graphics{
    
    void MatrixRect::_init( ci::ivec2 size, vec2 spacing ){
        
        hrfm::structure::ValueMatrix<ColorA> * mtx = new hrfm::structure::ValueMatrix<ColorA>();
        mtx->init( size.x, size.y );
        
        int col = size.x;
        int row = size.y;
        for( int y=0; y<row; y++ ){
            for( int x=0; x<col; x++ ){
                mtx->setValue( x, y, ColorA( 1.0, 1.0, 1.0, 1.0 ) );
            }
        }
        
        _init( mtx, spacing );
        
    }
    
    void MatrixRect::_init( hrfm::structure::ValueMatrix<ColorA> * matrix, vec2 spacing ){
        _mtx     = matrix;
        _spacing = spacing;
    }
    
    void MatrixRect::_draw(){
        
        Rectf bounds = getDrawBounds();
        
        int col = _mtx->getCol();
        int row = _mtx->getRow();
        
        vec2 size = vec2(
            (width -(_spacing.x*(float)(col-1))) / (float)col,
            (height-(_spacing.y*(float)(row-1))) / (float)row
        );
        
        for( int y=0; y<row; y++ ){
            for( int x=0; x<col; x++ ){
                Rectf rect = Rectf();
                rect.x1 = bounds.x1 + x*size.x + _spacing.x * x;
                rect.y1 = bounds.y1 + y*size.y + _spacing.y * y;
                rect.x2 = rect.x1 + size.x;
                rect.y2 = rect.y1 + size.y;
                ci::gl::color( _mtx->getValue( x, y ) );
                ci::gl::drawSolidRect(rect);
            }
        }
        
    }

}};