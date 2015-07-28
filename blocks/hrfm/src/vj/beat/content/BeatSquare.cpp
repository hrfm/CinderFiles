//
//  BeatSquare.cpp
//  RJ2015
//
//  Created by KAWAKITA Hirofumi on 7/27/15.
//
//

#include "BeatSquare.h"

namespace hrfm{ namespace vj{
    
    void BeatSquare::draw( double position, int width, int height ){
        
        float f = 0.5;
        float posA = min( 1.0, position / f );
        float posB = max( 0.0, ( position - (1.0-f) ) / f );
        
        ci::gl::color(1.0,1.0,1.0);
        Rectf rect = Rectf(0,0,width,height);
        rect.scaleCentered( posA );
        ci::gl::drawSolidRect( rect );
        
        ci::gl::color(0.0,0.0,0.0);
        rect = Rectf(0,0,width,height);
        rect.scaleCentered( posB );
        ci::gl::drawSolidRect( rect );
        
    }
    
    void BeatLine::draw( double position, int width, int height ){
        
        float f = 0.5;
        float posA = min( 1.0, position / f );
        float posB = max( 0.0, ( position - (1.0-f) ) / f );
        
        ci::gl::color(1.0,1.0,1.0);
        Rectf rect = Rectf( 0, 0, width*posA, height );
        ci::gl::drawSolidRect( rect );
        
        ci::gl::color(0.0,0.0,0.0);
        rect = Rectf( 0, 0, width*posB ,height );
        ci::gl::drawSolidRect( rect );
        
    }
    
    void BeatCircle::draw( double position, int width, int height ){
        
        float f = 0.8;
        float posA = position;
        float posB = max( 0.0, ( position - (1.0-f) ) / f );
        
        ci::gl::color(1.0,1.0,1.0);
        ci::gl::drawSolidCircle( Vec2f(width/2.0,height/2.0), width*posA/2.0 );
        
        ci::gl::color(0.0,0.0,0.0);
        ci::gl::drawSolidCircle( Vec2f(width/2.0,height/2.0), width*posB/2.0 );
        
    }
    
}}