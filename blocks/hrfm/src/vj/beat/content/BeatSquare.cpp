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
        
        /*
        float f = 0.6;
        float posA = min( 1.0, position / f );
        float posB = max( 0.0, ( position - (1.0-f) ) / f );
        posA *= posA;
        posB *= posB;
        /*/
        float f = 0.8;
        float posA = position;
        float posB = max( 0.0, ( position - (1.0-f) ) / f );
        //*/
        
        glPushMatrix();
        {
            
            ci::gl::translate( Vec2f( width/2.0f, height/2.0f ) );
            //ci::gl::rotate( 360.0f * position );
            
            ci::gl::color(1.0,1.0,1.0);
            Rectf rect = Rectf(-width/2.0f,-height/2.0f,width/2.0f,height/2.0f);
            rect.scaleCentered( posA );
            ci::gl::drawSolidRect( rect );
            
            ci::gl::color(0.0,0.0,0.0);
            rect = Rectf(-width/2.0f,-height/2.0f,width/2.0f,height/2.0f);
            rect.scaleCentered( posB );
            ci::gl::drawSolidRect( rect );
            
        }
        glPopMatrix();
        
    }
    
    void BeatLine::draw( double position, int width, int height ){
        
        /*
        float f = 0.5;
        float posA = min( 1.0, position / f );
        float posB = max( 0.0, ( position - (1.0-f) ) / f );
        posA *= posA;
        posB *= posB;
        /*/
        float f = 0.8;
        float posA = position;
        float posB = max( 0.0, ( position - (1.0-f) ) / f );
        //*/
        
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
    
    void BeatHoge::setup(){
        
        _order.clear();
        
        if( _random ){ _type = randInt(3); }
        
        if( _type == 0 ){
            _order.push_back( _rect0 );
            _order.push_back( _rect1 );
            _order.push_back( _rect2 );
            _order.push_back( _rect3 );
        }else if( _type == 1 ){
            _order.push_back( _rect3 );
            _order.push_back( _rect0 );
            _order.push_back( _rect1 );
            _order.push_back( _rect2 );
        }else{
            _order.push_back( _rect2 );
            _order.push_back( _rect0 );
            _order.push_back( _rect1 );
            _order.push_back( _rect3 );
        }
        
    }

    
    void BeatHoge::draw( double position, int width, int height ){
        
        float s = 0.1;
        float t = 0.6;
        float delay = 0.1;
        
        for( int i=0; i<4; i++ ){
            if( s < position && position < s + t ){
                float f = ( t - (position-s) ) / t;
                f = f*f;
                ci::gl::color(f,f,f);
                ci::gl::drawSolidRect( _order.at(i).scaled( Vec2f( width, height ) ) );
            }
            s+= delay;
        }
        
    }
    
    void BeatEqualizer::draw( double position, int width, int height ){
        
        hrfm::io::SiAudioInput * audio = &hrfm::io::SiAudioInput::getInstance();
        
        int length = 64;
        float w = (float)width / (float)( length - 1 );
        float h = (float)height;
        
        for( int i = 0; i < (length-1); i++ ) {
            
            float f = audio->getFFT()[i];
            float barY = f * height/1.6;
            
            ci::gl::color(f,f,f);
            
            Rectf rect = Rectf( i*w, h/2.0 - barY/2.0, (i+1)*w, h/2.0 + barY/2.0 );
            rect.scaleCentered( Vec2f(0.7, 1.0) );
            ci::gl::drawSolidRect( rect );
        }
        
    }
    
    void BeatAudioWave::draw( double position, int width, int height ){
        
        ci::gl::color(1.0,1.0,1.0);
        
        hrfm::io::SiAudioInput * audio = &hrfm::io::SiAudioInput::getInstance();
        
        int16_t buffLength = audio->getBufferLength();
        float w = (float)width  / (float)buffLength;
        float h = (float)height / 2.0;
        
        PolyLine<Vec2f>	leftBufferLine;
        
        ci::gl::lineWidth(10.0);
        glPushMatrix();
        glTranslatef( 0, h, 0.0f );
        {
            leftBufferLine.push_back( Vec2f( 0, 0 ) );
            for( int i = 0; i < buffLength; i++ ) {
                float x = i * w;
                float y = audio->getChannelAt(0)[i] * h;
                if( syncWithPosition ){
                    y *= position;
                }
                leftBufferLine.push_back( Vec2f( x , y ) );
            }
            leftBufferLine.push_back( Vec2f( width , 0 ) );
            ci::gl::draw( leftBufferLine );
        }
        glPopMatrix();
        ci::gl::lineWidth(1.0);

    }
    
    void BeatCapture::draw( double position, int width, int height ){
        ci::gl::color(1.0,1.0,1.0);
        /*
        for( int i=0; i<192; i++ ){
            for( int j=0; j<108; j++ ){
                ci::gl::drawCube(Vec3f(1000.0+0.1*i,100.0+0.1*j,0.0), Vec3f(1.0,1.0,1.0));
            }
        }
        */
    }
    
}}