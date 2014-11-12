#pragma once

#include "cinder/app/AppNative.h"
#include "Cinder/Camera.h"
#include "cinder/Xml.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

#include "cinder/Rand.h"

// --- Inputs ---
#include "Stage.h"
#include "Texture.h"

#include "SiOscInput.h"
#include "SiAudioInput.h"
#include "CaptureInput.h"

#include "TriggerEvent.h"
#include "SiEventDispatcher.h"

#include "Utils.h"

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
        }
        ~AppBase(){}
        
        // ===========================================================================
        // === PROPERTY. =============================================================
        
        // --- Common.
        
        XmlTree settingXml;
        
        CameraPersp camera;
        
        bool useOsc;
        bool useAudio;
        bool useCapture;
        bool doUpdateCapture;
        
        int nanoKontrolFader[16];
        
        hrfm::io::CaptureInput captureInput;
        
        hrfm::display::Stage stage;
        hrfm::display::Texture background;
        
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
        
        virtual void initFader();
        
        virtual void initWindow( XmlTree &xml );
        
        virtual void initOSC( XmlTree &xml );
        
        virtual void initAudio( XmlTree &xml );
        
        virtual void initCapture( XmlTree &xml );
        
        //! @override
        virtual void update();
        
        virtual void updateIO();
        
        virtual void updateStage();
        
        //! @override
        virtual void draw();
        
        // ===========================================================================
        // === Setter Getter =========================================================
        
        //! Set General Color.
        virtual void   setColor( float red, float green, float blue );
        
        //! Get General Color.
        virtual Color  getColor();
        
        //! Get General Color with alpha.
        virtual ColorA getColorA( float alpha );
        
        virtual Vec2i getWindowSize();
        
        virtual Vec2i getWindowAspect();
        
        virtual float getFaderValueAt( int index );
        
        // ===========================================================================
        // === Utility ===============================================================
        
        /**
         * node_name で指定したノード名で設定されている XML を読み込みます.
         */
        XmlTree loadXml( string node_name );
        XmlTree loadXml( string node_name, string local_path );
        
        // ===========================================================================
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
        
        // ===========================================================================
        // === For Debug. ============================================================
        
        /**
         * @virtual
         */
        virtual void drawFPS( Vec2f position = Vec2f(20.0f,20.0f) );
        
    protected:
        
        // ===========================================================================
        // === Member. ===============================================================
        
        string  mRemoteUrl;
        
        Vec2i   mWindowSize;
        
        Color   mColor;
        
        // ===========================================================================
        // === Method. ===============================================================
        
        virtual void setupOsc();
        
        /**
         * OSC メッセージを受信した際の処理.
         * @virtual
         */
        virtual void onReceiveOscMessage( OscInputEvent * event );
        
        /**
         * TriggerEvent 発生時の処理.
         * @virtual
         */
        virtual void onTrigger( TriggerEvent * event );
        
    };
    
}}