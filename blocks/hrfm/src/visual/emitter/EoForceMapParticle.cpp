#include "EoForceMapParticle.h"

using namespace ci;
using namespace std;
using namespace hrfm::events;

namespace hrfm{ namespace visual{ namespace emitter{
    
    EoForceMapParticle::EoForceMapParticle(){
        EmittObjectBase();
    }
    
    EoForceMapParticle::EoForceMapParticle( float t, hrfm::cv::ForceMap * map ){
        EoForceMapParticle();
        init( t, map );
    }
    
    void EoForceMapParticle::init( float time, hrfm::cv::ForceMap * map ){
        
        EmittObjectBase::init( time );
        
        _forceMap    = map;
        _forceX      = 0.0;
        _forceY      = 0.0;
        _forceLength = 0.0;
        
    }
    
    float EoForceMapParticle::getForceLength(){
        return _forceLength;
    }
    
    void EoForceMapParticle::_update(){
        
        EmittObjectBase::_update();
        
        if( _forceMap ){
            Vec2f pow = _forceMap->getForceAtf( ( baseX + x ) / (float)ci::app::getWindowWidth(), ( baseY + y ) / (float)ci::app::getWindowHeight() );
            _forceX -= pow.x;
            _forceY -= pow.y;
        }
        
        _forceX *= 0.6;
        _forceY *= 0.6;
        _forceLength = sqrt( _forceX * _forceX + _forceY * _forceY );
        
    }
    
    void EoForceMapParticle::_draw(){
        
        colorA.a = sin( _PI * _alpha );
        
        ci::gl::color( colorA );
        
        Vec2f center = Vec2f( baseX + x, baseY + y );
        
        int num = 10;
        
        ci::gl::enableAlphaBlending();
        glBegin( GL_TRIANGLE_FAN );
        {
            
            float r = _forceLength;
            float rad, c, s;
            float step = 360.f / (float)num * _PI / 180.f;
            
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
    
}}};