#pragma once

#include "hrfm.h"
#include "cinder/gl/gl.h"
#include "PolyLine.h"

namespace hrfm{ namespace graphics{
    
    // ------------------------------------------------------
    
    class DelayLinePoint{
    public:
        DelayLinePoint( vec3 position = vec3(0.0), double duration = 0.0 ){
            this->position  = position;
            this->duration  = duration;
        }
        vec2 getPosVec2(){ return vec2( this->position.x, this->position.y ); }
        vec3 position;
        double duration;
    };
    
    // ------------------------------------------------------
    
    class DelayLinesEvent : public hrfm::events::Event{
    public:
        DelayLinesEvent( const std::string &type, vector<vec3> * triangle = NULL ):Event(type), triangle(triangle){}
        ~DelayLinesEvent(){}
        Event clone(){
            DelayLinesEvent evt( type(), triangle );
            return evt;
        }
        vector<vec3> * triangle;
    };
    
    // ------------------------------------------------------
    
    class DelayLines : public DisplayNode{
        
    public:
        
        DelayLines( int length=10, float thickness=14.0, vec3 pos=vec3(0.0) ):DisplayNode(){
            this->_points.push_back( new DelayLinePoint(pos) );
            _line = new hrfm::graphics::PolyLine(thickness);
            for( int i=0; i<length; i++ ){
                _line->points.push_back( this->_points.at(0)->position );
            }
            this->addChild(_line);
        };
        ~DelayLines(){};
        
        void moveTo( vec3 position, double duration );
        void offsetTo( vec3 position, double duration );
        void addPoint( DelayLinePoint * point );
        vec3 getEndPosition();
        
    protected:
        
        virtual void _update();
        virtual void _draw();
        
        double _startTime;
        bool   _completed;
        int    _index = 0;
        std::vector<DelayLinePoint*> _points;
        hrfm::graphics::PolyLine * _line;
        
    private:
        
    };
    
}}