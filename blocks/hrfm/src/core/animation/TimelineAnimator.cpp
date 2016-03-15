#include "TimelineAnimator.h"

namespace hrfm{ namespace animation{
    
    int numAnimation();
    
    int TimelineAnimator::numAnimation(){
        return _list.size();
    }
    
    void TimelineAnimator::add( float timeScale, std::function<void(float)> func ){
        _totalTimeScale += timeScale;
        _list.push_back( std::pair<float,std::function<void(float)>>( timeScale, func ) );
    }
    
    void TimelineAnimator::animate( float timeScale ){
        if( timeScale < 0.0f || 1.0f < timeScale){
            return;
        }
        float start = 0.0;
        for( auto it = std::begin(_list); it != std::end(_list); ++it ){
            auto ts  = (*it).first;
            auto s   = start / _totalTimeScale;
            auto e   = (start+ts) / _totalTimeScale;
            if( s <= timeScale && timeScale <= e ){
                (*it).second((timeScale-s)/(e-s));
                return;
            }
            start += ts;
        }
    }
    
    void TimelineAnimator::animateAt( int idx, float timeScale ){
        int len = _list.size();
        if( len == 0 || idx < 0 || len <= idx ){
            return;
        }
        if( timeScale < 0.0f ){
            _list.at(idx).second(0.0);
        }else if( 1.0f < timeScale ){
            _list.at(idx).second(1.0);
        }else{
            _list.at(idx).second(timeScale);
        }
    }
    
}};