#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/Vbo.h"

#include "hrfm.h"

namespace hrfm{ namespace graphics{
    
    class PolyCircle:public hrfm::display::DisplayNode{
        
    public:
        
        PolyCircle( float thickness = 20.0f, float beginDegree = 0.0f, float endDegree = 360.0f, int step = 36, float drawStep = 1.0f ):hrfm::display::DisplayNode(){
            _init( thickness );
            setDrawRange( beginDegree, endDegree, step, drawStep );
        }
        
        void setDrawRange( float beginDegree = 0.0f, float endDegree = 360.0f, int step = 36, float drawStep = 1.0f );
        void setDrawStep( int step );
        void setDrawStep( float step );
        
        void setThickness( float thickness );
        
        // vboMode を使うと 描画を VBO にして高速化できる。
        // だがその代わり、更新が重くなる。というようにしたい。
        //void setVboMode( bool flag );
        
    protected:
        
        void _init( float thickness );
        virtual void _update();
        virtual void _draw();
        
        vector<vec3> _vertices;
        
        //bool _isVboMode;
        //ci::gl::VboMeshRef _vboMesh = NULL;
        
        float _thickness;
        float _beginDegree;
        float _endDegree;
        int   _step;
        int _drawStep;
        
    };
    
}}