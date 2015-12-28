#pragma once

#include "cinder/app/App.h"
#include "hrfm.h"

namespace hrfm{ namespace graphics{
    
    class LineGraph:public hrfm::display::DisplayNode{
        
    public:
        
        LineGraph():hrfm::display::DisplayNode(){
            _init();
        }
        
        virtual void setData( vector<float> list );
        virtual void setData( string key, vector<float> list );
        
        virtual void setAnimateSpeed( float speed );
        
        // 設定すると上下にプルプルする
        virtual void setNoise( float noise );
        
    protected:
        
        void _init();
        virtual void _update();
        virtual void _draw();
        
        map<string,vector<float>> _data;
        map<string,vector<float>> _currentData;
        
        float _speed;
        float _noise;
        
    };
    
}}