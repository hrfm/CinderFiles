#include "ColorUtil.h"

namespace hrfm{ namespace utils{
    
    /*
     
     色に関する便利関数を提供する Utility クラス.
     
     */
    ci::Color ColorUtil::HSVtoColor( const color::HSV& hsv ){
        
        const float h = hsv.h - floor(hsv.h);
        const float s = hsv.s;
        const float v = hsv.v;
        const float hueF = h * 6.0f;
        const int hueI = static_cast<int>(hueF);
        const float fr = hueF - hueI;
        const float m = v * (1.0f-s);
        const float n = v * (1.0f-s*fr);
        const float p = v * (1.0f-s*(1.0f-fr));
        
        ci::Color rgb;
        
        switch(hueI){
            case 0: rgb.r = v; rgb.g = p; rgb.b = m; break;
            case 1: rgb.r = n; rgb.g = v; rgb.b = m; break;
            case 2: rgb.r = m; rgb.g = v; rgb.b = p; break;
            case 3: rgb.r = m; rgb.g = n; rgb.b = v; break;
            case 4: rgb.r = p; rgb.g = m; rgb.b = v; break;
            default: rgb.r = v; rgb.g = m; rgb.b = n; break;
        }
        return rgb;
        
    }
    
    color::HSV ColorUtil::ColortoHSV( const ci::Color& rgb ){
        
        const float min = std::min(std::min(rgb.r,rgb.g),rgb.b);
        const float max = std::max(std::max(rgb.r,rgb.g),rgb.b);
        
        color::HSV hsv(0.0f,0.0f,max);
        
        const float delta = max - min;
        
        if(delta!=0.0f)
        {
            hsv.s = delta / max;
            
            if(rgb.r==max)
            {
                hsv.h = (rgb.g-rgb.b) / delta;
            }
            else if(rgb.g==max)
            {
                hsv.h = 2.0f + (rgb.b-rgb.r) / delta;
            }
            else
            {
                hsv.h = 4.0f + (rgb.r-rgb.g) / delta;
            }
            
            hsv.h /= 6.0f;
            
            if(hsv.h<0.0f)
            {
                hsv.h += 1.0f;
            }
        }
        
        return hsv;
        
    }
    
}}