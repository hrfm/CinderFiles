#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "Cinder/Camera.h"
#include "cinder/Xml.h"
#include "cinder/Rand.h"

// --- Inputs ---

#include "SiSetting.h"

#include "Stage.h"
#include "ImageTexture.h"

#include "SiOscInput.h"
#include "SiAudioInput.h"
#include "SiKORGMIDIInterface.h"
#include "CaptureInput.h"

#include "TriggerEvent.h"
#include "SiEventDispatcher.h"

#include "Utils.h"
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
    class AppBase : public ci::app::App, public hrfm::events::EventDispatcher{
        
    public:
        
        AppBase() : ci::app::App(), hrfm::events::EventDispatcher(){
            this->_isDebugMode = false;
            this->___font___   = Font("Helvetica",12);
        }
        ~AppBase(){}
        
        // ===========================================================================
        // === PROPERTY. =============================================================
        
        bool useAudio;
        bool useCapture;
        bool doUpdateCapture;
        
        hrfm::display::Stage stage;
        
        // 以下は必須ではないので最終的に外したい
        
        hrfm::io::CaptureInput captureInput;
        CameraPersp camera;
        
        // ===========================================================================
        // === METHOD. ===============================================================
        
        virtual void setup();
        virtual void initWindow( XmlTree &xml );
        virtual void initAudio( XmlTree &xml );
        virtual void initCapture( XmlTree &xml );
        virtual void initKORGMIDIInterface( XmlTree &xml );
        
        virtual ivec2 getWindowSize();
        virtual ivec2 getWindowAspect();
        
        virtual void update();
        virtual void updateIO();
        virtual void updateStage();
        
        //! @override
        virtual void draw();
        
        // for debug
        virtual void drawFPS( vec2 position = vec2(20.0f,20.0f) );
        void coutGLInfo();
        
        // === Event Listener. =======================================================
        
        // アプリのウィンドウサイズに変更があった際の処理.
        // camera のパースペクティブをウィンドウのアスペクト比にあわせる.
        virtual void resize();
        
        //! @override
        virtual void keyDown( ci::app::KeyEvent event );
         
    protected:
        
        ivec2 mWindowSize;
        bool _isDebugMode;
        bool _hideCursor;
        
        // TriggerEvent 発生時の処理.
        virtual void onTrigger( TriggerEvent * event );
        
        void _drawDebug();
        
    private:
        
        FILE * LOG_FILE;
        Font ___font___;
        
    };
    
}}