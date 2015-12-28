#include "LineGraph.h"

namespace hrfm{ namespace graphics{
    
    void LineGraph::setData( vector<float> data ){
        setData( "__default__", data );
    }
    
    void LineGraph::setData( string key, vector<float> data ){
        _data[key] = data;
        if( _currentData.find(key) == _currentData.end() ){
            vector<float> vec;
            _currentData[key] = vec;
        }
        _currentData[key].resize(_data[key].size());
    }
    
    void LineGraph::setAnimateSpeed( float speed ){
        if( speed < 0.0 ){
            speed = 0.0;
        }else if( 1.0 < speed ){
            speed = 1.0;
        }
        _speed = speed;
    }
    
    void LineGraph::setNoise( float noise ){
        if( noise < 0.0 ){
            noise = 0.0;
        }
        _noise = noise;
    }
    
    void LineGraph::_init(){
        _speed = 0.1;
        _noise = 0.0;
    }
    
    void LineGraph::_update(){
        for(auto it=std::begin(_data); it!=std::end(_data); it++ ){
            string key = it->first;
            int length = it->second.size();
            for( int i=0; i<length; i++ ){
                float v = it->second[i];
                if( 0.0 < _noise ){
                    v += randFloat( -_noise/2.0, _noise/2.0 );
                }
                if( v < 0.0 ){
                    v = 0.0;
                }else if( 1.0 < v ){
                    v = 1.0;
                }
                _currentData[key][i] += ( v - _currentData[key][i] ) * _speed;
            }
        }
    }
    
    void LineGraph::_draw(){
        
        //Rectf bounds = getDrawBounds();
        //ci::gl::drawLine( bounds.getUpperLeft(), bounds.getLowerLeft() );
        //ci::gl::drawLine( bounds.getLowerLeft(), bounds.getLowerRight() );
        
        for(auto it=std::begin(_currentData); it!=std::end(_currentData); it++ ){
            int len = it->second.size();
            if( 1 < len ){
                float w = width / (float)(len-1);
                PolyLineT<vec2>	line;
                for( int i = 0; i < len; i++ ) {
                    float x = (float)i * w;
                    float y = it->second.at(i) * height;
                    line.push_back( vec2( x , height - y ) );
                }
                ci::gl::draw( line );
            }
        }
    }
    
}}