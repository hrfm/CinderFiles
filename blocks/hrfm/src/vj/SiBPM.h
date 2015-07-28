#pragma once

#include "cinder/app/App.h"
#include "Singleton.h"
#include <EventDispatcher.h>

using namespace std;

namespace hrfm{ namespace vj{
    
    class SiBPM : public hrfm::utils::Singleton<SiBPM>, public hrfm::events::EventDispatcher{
        
    public:
        
        virtual void setBPM( double bpm );
        virtual double getBPM();
        
        virtual void start();
        virtual void start( double bpm );
        
        virtual void stop();
        
        virtual void update();
        
        double bpm;
        double millisecPerBeat;
        double position;
        double startTime;
        
    private:
        
        friend class hrfm::utils::Singleton<SiBPM>;
        
        SiBPM(){
            hrfm::events::EventDispatcher();
            this->bpm = 120;
            this->millisecPerBeat = 60.00000 / bpm;
            this->startTime = 0;
        }
        
    };
    
}}