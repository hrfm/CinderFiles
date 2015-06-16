#pragma once

#include "cinder/app/AppBasic.h"

using namespace ci;

namespace hrfm{ namespace cv{
    
    // ========================================================================================= //
    
    class DetectRect : public Rectf{
        
        public :
        
        DetectRect( float x1, float y1, float x2, float y2 );
        DetectRect( Rectf rect );
        
        void update();
        
        bool check( Rectf rect );
        
        bool dead();
        
        int time;
        
        private :
        
        void setup( Rectf rect, int lifespan = 12 );
        
        int life;
        int lifeMax;
        Rectf targetRect;
        
    };
    
}}