#pragma once

#include "hrfm.h"
#include "hrfm.gl.h"
#include "hrfm.signage.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace hrfm{ namespace app{
    
    class SignageAppBase : public hrfm::app::AppBase{
    
    public:
        
        SignageAppBase():hrfm::app::AppBase(){}
        ~SignageAppBase(){}
        
        virtual void setup();
        
        hrfm::signage::display::ScheduledContents  * scheduledContents  = NULL;
        hrfm::signage::display::SequentialContents * sequentialContents = NULL;
        
    protected:
        
        virtual void _initScheduled( XmlTree settings );
        virtual void _initSequential( XmlTree settings );
        
        virtual void onTrigger( hrfm::events::TriggerEvent * event );
        virtual void onScheduleComplete( hrfm::events::Event * event );
        
    };
    
}}