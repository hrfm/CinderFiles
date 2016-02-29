#pragma once

#include "cinder/app/App.h"

namespace hrfm{ namespace utils{
    
    namespace color{
        class HSV{
        public:
            HSV(){}
            HSV( float h, float s, float v ){
                this->h = h;
                this->s = s;
                this->v = v;
            }
            float h,s,v;
        };
    }
    
    /*
     
     色に関する便利関数を提供する Utility クラス.
     
     */
    class ColorUtil{
    public:
        static ci::Color  HSVtoColor( const color::HSV& hsv );
        static color::HSV ColortoHSV( const ci::Color&  rgb );
    private:
    };
    
}}