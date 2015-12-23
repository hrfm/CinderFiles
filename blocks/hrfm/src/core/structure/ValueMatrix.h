#pragma once

#include "cinder/app/App.h"

namespace hrfm{ namespace structure{
    
    template <typename T>
    class ValueMatrix{
        
    public:
        
        ValueMatrix(){}
        
        ValueMatrix( int col, int row ){
            init( col, row );
        }
        
        ValueMatrix( ivec2 size ){
            init( size.x, size.y );
        }
        
        void init( int col, int row ){
            _col = col;
            _row = row;
            _data = std::vector<T>(_col*_row);
        }
        
        int getCol(){ return _col; }
        int getRow(){ return _row; };
        int getLength(){ return _data.size(); };
        
        T getValue( int index ){ return _data[index]; }
        T getValue( int col, int row ){ return _data[(row*_col)+col]; }
        
        void setValue( int index, T value ){
            _data[index] = value;
        }
        void setValue( int col, int row, T value ){
            _data[(row*_col)+col] = value;
        }
        
    protected:
        
        int _col;
        int _row;
        std::vector<T> _data;
        
    };
    
}}