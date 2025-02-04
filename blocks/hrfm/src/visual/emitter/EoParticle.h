#pragma once

#include "hrfm.h"
#include "EoForceMapParticle.h"

using namespace ci;
using namespace std;
using namespace hrfm::events;

namespace hrfm{ namespace visual{ namespace emitter{
    
    class EoParticle :public EoForceMapParticle{
        
    public:
        
        EoParticle( float t, hrfm::cv::ForceMap * map ){
            EoForceMapParticle();
            init( t, map );
        }
        
        virtual void init( float time, hrfm::cv::ForceMap * map ){
            
            EoForceMapParticle::init( time, map );
            
            hrfm::io::SiAudioInput * audioInput = &hrfm::io::SiAudioInput::getInstance();
            
            float pow = 10.0f * audioInput->getFFTRangedAt(1);
            if( 20.0f < pow ) pow = 10.0f;
            
            _radius = 0.3 + pow;
            _idx    = floor( 5.0 * audioInput->getFFTRangedAt(1) );
            _vecX   = randFloat( - pow * 3.0, pow * 3.0 );
            _vecY   = -randFloat( pow );
            _accX   = 0.0;
            _accY   = - ( - 0.2 - randFloat() ) * pow / 10.0f;
            
        }
        
    protected:
        
        void _update(){
            
            EoForceMapParticle::_update();
            
            x += _vecX;
            y += _vecY;
            
            _vecX   *= 0.96;
            _vecY   *= 0.96;
            _forceX *= 0.6;
            _forceY *= 0.6;
            
            _vecX += _accX + _forceX * 0.4;
            _vecY += _accY + _forceY * 0.4;
            
        }
        
        void _draw(){
            
            hrfm::io::SiAudioInput * audioInput = &hrfm::io::SiAudioInput::getInstance();
            
            colorA.a = sin( _PI * _alpha );
            
            ci::gl::color( colorA );
            
            Vec2f center = Vec2f( baseX + x, baseY + y );
            
            int num = 10;
            
            ci::gl::enableAlphaBlending();
            glBegin( GL_TRIANGLE_FAN );
            {
                
                float r = _radius * 10.0f * audioInput->getFFTRangedAt(_idx);
                if( 20.0f < r ){
                    r = 20.0f;
                }
                
                float rad, c, s;
                float step = 360.f / (float)num * _PI / 180.f;
                
                ci::gl::vertex( Vec2f( center.x, center.y ) );
                for( int i=0; i <= num; i++ ){
                    rad = step * (float)i;
                    c   = cos(rad);
                    s   = sin(rad);
                    ci::gl::vertex( Vec2f( center.x + r*c, center.y + r*s ) );
                }
                
                r += 10.0 * audioInput->getFFTRangedAt(1);
                colorA.a *= 0.5;
                ci::gl::color( colorA );
                
                ci::gl::vertex( Vec2f( center.x, center.y ) );
                for( int i=0; i <= num; i++ ){
                    rad = step * (float)i;
                    c   = cos(rad);
                    s   = sin(rad);
                    ci::gl::vertex( Vec2f( center.x + r*c, center.y + r*s ) );
                }
                
            }
            glEnd();
            ci::gl::disableAlphaBlending();
            
        }
        
    private:
        
        float _vecX;
        float _vecY;
        float _accX;
        float _accY;
        
        float _radius;
        int   _idx;
        
    };
    
}}};