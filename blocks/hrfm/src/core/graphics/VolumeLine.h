#pragma once

#include "cinder/app/App.h"
#include "hrfm.h"

namespace hrfm{ namespace graphics{
    
    class VolumeLine:public hrfm::display::DisplayNode{
        
    public:
        
        VolumeLine():hrfm::display::DisplayNode(){}
        
        virtual void setFromTo( vec2 fromPos, float fromRadius, vec2 toPos, float toRadius );
        virtual void setNextTo( vec2 pos );
        virtual void setNextTo( vec2 pos, float radius );
        virtual void offsetTo( vec2 offset );
        virtual void offsetTo( vec2 offset, float radius );

    protected:
        
        virtual float _getRadian();
        
        virtual void _update();
        virtual void _draw();
        
        vec3  _fromPos;
        float _fromRadius;
        vec3  _toPos;
        float _toRadius;
        
        vector<vec3> _vertices;
        
    };
    
}}