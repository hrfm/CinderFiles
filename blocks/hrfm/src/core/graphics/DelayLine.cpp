#include "DelayLine.h"

namespace hrfm{ namespace graphics{
    
    void DelayLines::moveTo( vec3 position, double duration ){
        addPoint( new DelayLinePoint( position, duration ) );
    }
    
    void DelayLines::offsetTo( vec3 position, double duration ){
        addPoint( new DelayLinePoint( _points[_points.size()-1]->position + position, duration ) );
    }
    
    void DelayLines::addPoint( DelayLinePoint * point ){
        if( this->_points.size()-1 == _index || _completed ){
            _startTime = ci::app::getElapsedSeconds();
        }
        this->_points.push_back( point );
        _reached      = false;
        _reachedCount = 0;
        _completed    = false;
    }
    
    vec3 DelayLines::getEndPosition(){
        return _points[_points.size()-1]->position;
    }
    
    void DelayLines::_update(){
        
        if( _completed ){
            return;
        }
        
        vec3 pos;
        
        if( _points.size() == 1 ){
            
            pos   = _points.at(0)->position;
            
        }else if( _points.size() - 1 <= _index ){
            
            pos = _points.at(_index)->position;
            
        }else{
            
            vec3 p0 = _points.at(_index)->position;
            vec3 p1 = _points.at(_index+1)->position;
            double duration = _points.at(_index+1)->duration;
            
            double now = ci::app::getElapsedSeconds();
            double p = ( now - _startTime ) / duration;
            
            if( 1.0 <= p ){
                p = 1.0;
                _index++;
                _reachedCount = 0;
                _startTime = ci::app::getElapsedSeconds();
            }
            
            pos = p0 + (p1-p0)*vec3(p);
            
        }
        
        for( int i=0; i<_line->points.size()-1; i++ ){
            _line->points[i] = _line->points[i+1];
        }
        _line->points[_line->points.size()-1] = pos;
        
        if( _points.size()-1 <= _index ){
            if( !_reached ){
                _reached = true;
                dispatchEvent( new hrfm::graphics::DelayLinesEvent("reached") );
            }
            if( !_completed && _line->points.size() <= ++_reachedCount ){
                auto last = this->_points.at( this->_points.size()-1 );
                this->_points.empty();
                this->_points.clear();
                this->_points.push_back(last);
                _index = 0;
                _completed = true;
                dispatchEvent( new hrfm::graphics::DelayLinesEvent("end") );
            }
        }
        
    };
    
    void DelayLines::_draw(){};
    
}}