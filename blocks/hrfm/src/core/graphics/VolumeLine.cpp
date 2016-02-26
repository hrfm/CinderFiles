#include "VolumeLine.h"

namespace hrfm{ namespace graphics{
    
    void VolumeLine::setFromTo( vec2 fromPos, float fromRadius, vec2 toPos, float toRadius ){
        _fromPos    = vec3(fromPos,0.0);
        _fromRadius = fromRadius;
        _toPos      = vec3(toPos,0.0);
        _toRadius   = toRadius;
    }
    
    void VolumeLine::setNextTo( vec2 pos ){
        _fromPos    = vec3(_toPos);
        _fromRadius = _toRadius;
        _toPos      = vec3(pos,0.0);
    }
    
    void VolumeLine::setNextTo( vec2 pos, float radius ){
        _fromPos    = vec3(_toPos);
        _fromRadius = _toRadius;
        _toPos      = vec3(pos,0.0);
        _toRadius   = radius;
    }
    
    void VolumeLine::offsetTo( vec2 offset ){
        _fromPos    = vec3(_toPos);
        _fromRadius = _toRadius;
        _toPos      += vec3(offset,0.0);
    }
    
    void VolumeLine::offsetTo( vec2 offset, float radius ){
        _fromPos    = vec3(_toPos);
        _fromRadius = _toRadius;
        _toPos      += vec3(offset,0.0);
        _toRadius   = radius;
    }
    
    float VolumeLine::_getRadian(){
        return atan2( _toPos.y - _fromPos.y, _toPos.x - _fromPos.x );
    }
    
    void VolumeLine::_update(){
        
        _vertices.empty();
        _vertices.clear();
        
        setPosition(_fromPos);
        
        float radian = _getRadian();
        float step = 3.14159265 / 12.0;
        
        if( distance(_fromPos,_toPos) <= 1.0 ){
            for( int i=0; i<24; i++ ){
                _vertices.push_back( vec3( _toRadius * cos( radian ), _toRadius * sin( radian ), 0.0 )  );
                radian -= step;
            }
            return;
        }else{
            if( _fromRadius <= 1.0 ){
                if( _toRadius <= 1.0 ){
                    return;
                }
                _vertices.push_back( vec3(0.0) );
            }else{
                float rad = radian - 3.14159265/2.0;
                for( int i=0; i<=12; i++ ){
                    _vertices.push_back( vec3( _fromRadius * cos( rad ), _fromRadius * sin( rad ), 0.0 )  );
                    rad -= step;
                }
            }
            if( _toRadius <= 1.0 ){
                _vertices.push_back( (_toPos-_fromPos) );
            }else{
                float rad = radian + 3.14159265/2.0;
                for( int i=0; i<=12; i++ ){
                    _vertices.push_back( (_toPos-_fromPos) + vec3( _toRadius*cos(rad), _toRadius*sin(rad), 0.0 ) );
                    rad -= step;
                }
            }
        }
        
    }
    
    void VolumeLine::_draw(){
        if( 0 < _vertices.size() ){
            ci::gl::begin(GL_TRIANGLE_FAN);
            {
                for( int i=0; i<=_vertices.size(); i++ ){
                    ci::gl::vertex( _vertices[i] );
                }
            }
            ci::gl::end();
        }
    }
    
}}