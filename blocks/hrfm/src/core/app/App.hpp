#pragma once 

#include <iostream>

#include "cinder/app/App.h"
#include "cinder/Utilities.h"
#include "cinder/Xml.h"

#include "hrfm.fs.h"

#include "SiAppSettings.hpp"
#include "SiAppInfo.h"
#include "SiTimeUtil.h"

#include "EventDispatcher.h"
#include "SiEventDispatcher.h"
#include "TriggerEvent.h"

namespace hrfm { namespace app {
    
    class App : public ci::app::App, public hrfm::events::EventDispatcher{
        
    public:
        
        App() : ci::app::App(), hrfm::events::EventDispatcher(){
            this->_init();
        }
        ~App(){}
        
        virtual void setup() override;
        
        virtual void update() override;
        virtual void draw() override;
        virtual void drawFPS( ci::vec2 position = ci::vec2(20.0f,20.0f) );
        
        virtual void keyDown( ci::app::KeyEvent event ) override;
        
    protected:
        
        virtual void _init(){
            this->_isDebugMode = false;
        }
        
        bool _isDebugMode;
        virtual void _drawDebug();
        
        virtual void _setupFromXML( ci::XmlTree &xml );
        bool _hideCursorOnFullscreen;
        
        // TriggerEvent 発生時の処理.
        virtual void onTrigger( events::TriggerEvent * event );
        
    private:
        
        FILE * LOG_FILE;
        
    };
    
}}

/*
    // ===========================================================================
    // === PROPERTY. =============================================================
 
    hrfm::display::StageRef stage;
 
    // 以下は必須ではないので最終的に外したい
    CameraPersp camera;
    
    // ===========================================================================
    // === METHOD. ===============================================================
 
    virtual void updateStage();
 
    // === Event Listener. =======================================================
    
    // アプリのウィンドウサイズに変更があった際の処理.
    // camera のパースペクティブをウィンドウのアスペクト比にあわせる.
    virtual void resize();
 
protected:
    // TriggerEvent 発生時の処理.
    virtual void onTrigger( TriggerEvent * event );
 
*/
