#pragma once

#include "SiTextureMap.h"
#include "SiTimeUtil.h"
#include "XmlLoader.h"

namespace hrfm{ namespace utils{
    
    static int GCD( int a, int b ){
        int c;
        if (a < b) {
            a += b;
            b =  a-b;
            a -= b;
        }
        while (b != 0) {
            c = a % b;
            a = b;
            b = c;
        }
        return a;
    }
    
    static ci::Vec2i getAspectRatio( ci::Vec2i resolution ){
        return resolution / GCD( resolution.x, resolution.y );
    }
    
    static float sigmoid( float value, float gain ){
        return 1.0 / ( 1.0 + pow( M_E, - gain * value ) );
    }
    
}}