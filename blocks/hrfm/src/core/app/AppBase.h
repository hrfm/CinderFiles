#pragma once

#include "cinder/app/App.h"
#include "Cinder/Camera.h"
#include "cinder/Xml.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

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
    class AppBase : public ci::app::AppNative, public hrfm::events::EventDispatcher{
        
    public:
        
        AppBase(){
            ci::app::AppNative();
            hrfm::events::EventDispatcher();
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
        
        /**
         * ローカルにおいてある設定ファイル resoureces/setting.xml を読み込み
         * 主にモニタサイズやフレームレートなどハードウェアに関係した設定を行います.
         * @virtual
         * @override
         */
        virtual void prepareSettings( Settings *settings );
        
        //! @override
        virtual void setup();
        virtual void initWindow( XmlTree &xml );
        virtual void initAudio( XmlTree &xml );
        virtual void initCapture( XmlTree &xml );
        virtual void initKORGMIDIInterface( XmlTree &xml );
        
        virtual Vec2i getWindowSize();
        virtual Vec2i getWindowAspect();
        
        //! @override
        virtual void update();
        virtual void updateIO();
        virtual void updateStage();
        
        //! @override
        virtual void draw();
        
        /**
         # for debug.
         * @virtual
         */
        virtual void drawFPS( Vec2f position = Vec2f(20.0f,20.0f) );
        
        // === Event Listener. =======================================================
        
        /**
         * アプリのウィンドウサイズに変更があった際の処理.
         * camera のパースペクティブをウィンドウのアスペクト比にあわせる.
         * @virtual
         * @override
         */
        virtual void resize();
        
        //! @override
        virtual void keyDown( ci::app::KeyEvent event );
        
        
    protected:
        
        Vec2i mWindowSize;
        
        /**
         * TriggerEvent 発生時の処理.
         * @virtual
         */
        virtual void onTrigger( TriggerEvent * event );
        
    private:
        
        bool _isDebugMode;
        bool _hideCursor;
        
        FILE * LOG_FILE;
        Font ___font___;
        
    };
    
}}