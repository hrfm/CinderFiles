#pragma once

#include "EmittObjectBase.h"

#include "cinder/Rand.h"

using namespace ci;
using namespace std;
using namespace hrfm::events;

namespace hrfm{ namespace visual{ namespace emitter{
    
    class EoPolyLine :public EmittObjectBase{
    
    public:
        
        EoPolyLine( float t = 1.0f ){
            EmittObjectBase();
            init( t );
        }
        
        void init( float time ){
            EmittObjectBase::init( time );
            animateFrom();
            animateTo();
        }
        
        void animateFrom( float alpha = 1.0f, float radius = 0.0f, float thickness = 30.0f, int segments = 60, float start = 0.0f, float end = 1.0f, float rotate = 0.0f ){
            this->_alphaFrom     = alpha;
            this->_radiusFrom    = radius;
            this->_thicknessFrom = thickness;
            this->_segmentsFrom  = segments;
            this->_startFrom     = start;
            this->_endFrom       = end;
            this->_rotateFrom    = rotate;
        }
        
        void animateTo( float alpha = 0.0f, float radius = 1000.0f, float thickness = 30.0f, int segments = 60, float start = 0.0f, float end = 1.0f, float rotate = 0.0f ){
            this->_alphaTo     = alpha;
            this->_radiusTo    = radius;
            this->_thicknessTo = thickness;
            this->_segmentsTo  = segments;
            this->_startTo     = start;
            this->_endTo       = end;
            this->_rotateTo    = rotate;
        }
        
        
        void animateFromRandom(){
            animateFrom( 1.0f,    0.0f, randFloat(20.0f), 3 + randInt(40), randFloat(), randFloat(), randFloat( -3.0, 3.0 ) );
        }
        
        void animateToRandom(){
            animateTo  ( 0.0f, 1000.0f, 30.0f + randFloat(30.0f), 3 + randInt(40), randFloat(), randFloat(), randFloat( -3.0, 3.0 ) );
        }
        
    protected:
        
        void _update(){
            
            EmittObjectBase::_update();
            
            _radius    = _radiusFrom    + ( _radiusTo    - _radiusFrom    ) * progress;
            _thickness = _thicknessFrom + ( _thicknessTo - _thicknessFrom ) * progress;
            
            _segments  = round( (float)_segmentsFrom  + ( _segmentsTo  - _segmentsFrom  ) * progress );
            _start     = _startFrom     + ( _startTo     - _startFrom     ) * progress;
            _end       = _endFrom       + ( _endTo       - _endFrom       ) * progress;
            _rotate    = _rotateFrom    + ( _rotateTo    - _rotateFrom    ) * progress * _PI * 2;
            
            _step      = ( 360.f / (float)_segments ) * _PI / 180.f * ( _end - _start );
            
        }
        
        void _draw(){
            
            colorA.a = _alpha * _alpha;
            
            ci::gl::color( colorA );
            
            vec2 center = vec2( baseX + x, baseY + y );
            
            ci::gl::begin( GL_TRIANGLE_STRIP );
            {
                float r, rad, c, s;
                for( int i=0; i <= _segments; i++ ){
                    
                    rad = (-_rotate) + ( _start + _step * (float)i );
                    c   = cos(rad);
                    s   = sin(rad);
                    
                    r   = _radius + _thickness/2.0f;
                    ci::gl::vertex( vec2( center.x + r*c, center.y + r*s ) );
                    
                    r   = _radius - _thickness/2.0f;
                    if( r < 0 ){
                        r = 0;
                    }
                    ci::gl::vertex( vec2( center.x + r*c, center.y + r*s ) );
                    
                }
            }
            ci::gl::end();
            
        }
        
    private:
        
        float _radiusFrom;
        float _radiusTo;
        float _radius;
        
        float _thicknessFrom;
        float _thicknessTo;
        float _thickness;
        
        int   _segmentsFrom;
        int   _segmentsTo;
        int   _segments;
        
        float _stepFrom;
        float _stepTo;
        float _step;
        
        float _startFrom;
        float _startTo;
        float _start;
        
        float _endFrom;
        float _endTo;
        float _end;
        
        float _rotate;
        float _rotateFrom;
        float _rotateTo;
        
    };
    
    
}}}