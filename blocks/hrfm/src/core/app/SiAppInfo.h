#pragma once

#include "cinder/app/App.h"
#include "Singleton.h"
#include "SiAppSettings.hpp"

namespace hrfm { namespace app{
    
    class SiAppInfo : public hrfm::utils::Singleton<SiAppInfo>{
        
    public:
        
        void setElapsedSecondsByFrame( double seconds );
        
        void update();
        
        SiAppSettings & getAppSettings();
        
        double   getElapsedSeconds();
        double   getBeforeElapsedSeconds();
        double   getElapsedSecondsFromBeforeUpdate();
        uint32_t getElapsedFrames();
        
    private:
        
        friend class hrfm::utils::Singleton<SiAppInfo>;
        
        SiAppInfo(){}
        
        double   _elapsedSecondsByFrame = -1.0;
        double   _elapsedSeconds        = 0.0;
        double   _beforeElapsedSeconds  = 0.0;
        uint32_t _elapsedFrames         = 0;
        
    };
    
}}
