#pragma once

#include "hrfm.h"
#include "cinder/gl/gl.h"

namespace hrfm{ namespace graphics{
    
    class PolyLine:public DisplayNode{
    public:
        
        PolyLine(float thickness = 2.0):DisplayNode(){
            this->thickness = thickness;
        }
        ~PolyLine();
        
        float        thickness;
        vector<vec3> points;
        
        //void closePath();
        //bool _isPathClosed;
        
    protected:
        
        virtual void _update();
        virtual void _draw();
        
        vector<vec3> _vertices;
        
    };
    
}}