#pragma once

#include "hrfm.h"
#include "cinder/gl/gl.h"

namespace hrfm{ namespace graphics{
    
    class PolyLine:public DisplayNode{
    public:
        
        PolyLine( float thickness = 2.0 ):DisplayNode(){
            this->thickness = thickness;
            this->_lengthList.push_back(0.0);
        }
        ~PolyLine();
        
        virtual void addPoint( vec3 point );
        virtual void clear();
        
        virtual void setThickness( float thickness );
        virtual void setDrawRange( float from, float to );
        virtual void setPointScale( vec3 scale );
        virtual void setPointScale( vec3 scale, vec3 pivot );
        virtual void setPointScalePivot( vec3 pivot );
        
        float        thickness;
        vector<vec3> points;
        
    protected:
        
        virtual void _update();
        virtual void _draw();
        
        vector<vec3> _vertices;
        
        float _drawFrom = 0.0;
        float _drawTo   = 1.0;
        
        float _totalLength = 0.0;
        vector<float> _lengthList;
        
        bool _usePointScale = false;
        vec3 _pointScale;
        vec3 _pointScalePivot;
        
    };
    
}}