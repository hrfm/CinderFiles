#pragma once

#include "DynamicMap.h"
#include "DisplayNode.h"

namespace hrfm{ namespace animation{

    class TimelineAnimator : public hrfm::events::EventDispatcher{
        
    public:
        
        TimelineAnimator(  ):hrfm::events::EventDispatcher(){
            _totalTimeScale = 0.0;
        }
        
        void add( float timeScale, std::function<void(float)> func ){
            _totalTimeScale += timeScale;
            _list.push_back( std::pair<float,std::function<void(float)>>( timeScale, func ) );
        }
        
        void animate( float timeScale ){
            
            if( timeScale < 0.0f || 1.0f < timeScale){
                return;
            }
            
            float start = 0.0;
            for( auto it = std::begin(_list); it != std::end(_list); ++it ){
                auto ts  = (*it).first;
                auto fnc = (*it).second;
                auto s   = start / _totalTimeScale;
                auto e   = (start+ts) / _totalTimeScale;
                //cout << s << ":" << e << "  =  " << timeScale << endl;
                if( s <= timeScale && timeScale < e ){
                    fnc((timeScale-s)/(e-s));
                    return;
                }
                start += ts;
            }
            
        }
        
    protected:
        
        float _totalTimeScale;
        std::vector< std::pair< float, std::function<void(float)> > > _list;
        
    };
    
}};