#include "SiAppInfo.h"

using namespace ci;

namespace hrfm { namespace app{
    
    void  SiAppInfo::setElapsedSecondsByFrame( double seconds ){
        _elapsedSecondsByFrame = seconds;
    }
    
    void SiAppInfo::update(){
        _beforeElapsedSeconds = _elapsedSeconds;
        if( 0.0 < _elapsedSecondsByFrame ){
            _elapsedSeconds += _elapsedSecondsByFrame;
        }else{
            _elapsedSeconds = ci::app::getElapsedSeconds();
        }
        _elapsedFrames = ci::app::getElapsedFrames();
    }
    
    double SiAppInfo::getElapsedSeconds(){
        return _elapsedSeconds;
    }
    
    double SiAppInfo::getBeforeElapsedSeconds(){
        return _beforeElapsedSeconds;
    }
    
    double SiAppInfo::getElapsedSecondsFromBeforeUpdate(){
        return _elapsedSeconds - _beforeElapsedSeconds;
    }
    
    uint32_t SiAppInfo::getElapsedFrames(){
        return _elapsedFrames;
    }
    
    SiSetting & SiAppInfo::getSetting(){
        return SiSetting::getInstance();
    }
    
}}