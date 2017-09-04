#pragma once

#include "cinder/Capture.h"

#include "AppInstallation.hpp"
#include "ScheduledContents.h"
#include "Stage.h"
#include "XmlLoader.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace hrfm::display;
using namespace hrfm::io;

namespace hrfm{ namespace app{
    
    class AppSignage : public hrfm::app::AppInstallation{
    
    public:
        
        AppSignage():hrfm::app::AppInstallation(){}
        ~AppSignage(){}
        
        virtual ivec2 getWindowSize();
        
        virtual void setup();
        virtual void update();
        virtual void updateStage();
        virtual void draw();
        virtual void resize();
        
        // ----
        
        bool doUpdateCapture;
        
        hrfm::display::StageRef stage;
        ci::CaptureRef capture;
        
        // 以下は必須ではないので最終的に外したい
        
        CameraPersp camera;
        
        hrfm::signage::display::ScheduledContents  * scheduledContents  = NULL;
        hrfm::signage::display::SequentialContents * sequentialContents = NULL;
        
    protected:
        
        ivec2 mWindowSize;
        
        virtual void _init(){
            hrfm::app::App::_init();
            this->___font___   = Font("Helvetica",12);
        }
        
        virtual void _initScheduled( XmlTree settings );
        virtual void _initSequential( XmlTree settings );
        
        virtual void onScheduleStart( hrfm::signage::events::ScheduledContentsEvent * event );
        virtual void onScheduleComplete( hrfm::signage::events::ScheduledContentsEvent * event );
        
    private:
        
        Font ___font___;
        
    };
    
}}
