#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"
#include "cinder/Xml.h"
#include "cinder/Rand.h"
#include "cinder/Capture.h"

#include "AppInstallation.hpp"

// --- Inputs ---

#include "SiAppInfo.h"
#include "Stage.h"
#include "SiTimeUtil.h"
#include "XmlLoader.h"

using namespace hrfm;
using namespace hrfm::display;
using namespace hrfm::events;
using namespace hrfm::io;

namespace hrfm{ namespace app{
    
    /*
     
     Extended Cinder AppNative Class.
     Setup general behavior I always using.
     And capable configured by settings xml files.
     
     */
    class AppBase : public hrfm::app::AppInstallation{
        
    public:
        
        AppBase() : hrfm::app::AppInstallation(){}
        ~AppBase(){}
        
        // ===========================================================================
        // === PROPERTY. =============================================================
        
        bool doUpdateCapture;
        
        hrfm::display::StageRef stage;
        ci::CaptureRef capture;
        
        // 以下は必須ではないので最終的に外したい
        
        CameraPersp camera;
        
        // ===========================================================================
        // === METHOD. ===============================================================
        
        virtual void setup();
        
        virtual ivec2 getWindowSize();
        
        virtual void update();
        virtual void updateStage();
        
        //! @override
        virtual void draw();
        
        // === Event Listener. =======================================================
        
        // アプリのウィンドウサイズに変更があった際の処理.
        // camera のパースペクティブをウィンドウのアスペクト比にあわせる.
        virtual void resize();
        
    protected:
        
        ivec2 mWindowSize;
        
        virtual void _init(){
            hrfm::app::App::_init();
            this->___font___   = Font("Helvetica",12);
        }
        
    private:
        
        Font ___font___;
        
    };
    
}}
