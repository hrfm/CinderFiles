#include "DetectRect.h"

using namespace ci;

namespace hrfm{ namespace io{
    
    DetectRect::DetectRect( float x1, float y1, float x2, float y2 ){
        setup( Rectf(x1, y1, x2, y2) );
    }
    
    DetectRect::DetectRect( Rectf rect ){
        setup(rect);
    }
    
    void DetectRect::update(){
        if( 0 < life ){ life--; }
        x1 += ( targetRect.x1 - x1 ) * 0.98;
        y1 += ( targetRect.y1 - y1 ) * 0.98;
        x2 += ( targetRect.x2 - x2 ) * 0.98;
        y2 += ( targetRect.y2 - y2 ) * 0.98;
        time++;
    }
    
    bool DetectRect::check( Rectf rect ){
        // 距離が一定以内の場合は同一の顔とみなす.
        if( rect.distance( getCenter() ) < 10 ){
            targetRect.set( rect.x1, rect.y1, rect.x2, rect.y2 );
            life = lifeMax;
            return true;
        }
        return false;
    }
    
    bool DetectRect::dead(){
        if( life <= 0 ){
            //cout << "dead!!" << endl;
            return life <= 0;
        }else{
            return false;
        }
    }
    
    void DetectRect::setup( Rectf rect, int lifespan ){
        set( rect.x1, rect.y1, rect.x2, rect.y2 );
        targetRect.set( rect.x1, rect.y1, rect.x2, rect.y2 );
        life    = lifespan;
        lifeMax = lifespan;
        time    = 0;
    }
    
}}