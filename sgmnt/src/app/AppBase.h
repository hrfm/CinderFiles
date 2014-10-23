#pragma once





#include "cinder/app/AppNative.h"
#include "Cinder/Camera.h"
#include "cinder/Xml.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

// --- timeinfo ---
#include <time.h>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

#include "cinder/Rand.h"

// --- Inputs ---
#include "OscInput.h"
#include "AudioInput.h"
#include "CaptureInput.h"
#include "Utils.h"

using namespace sgmnt;
using namespace sgmnt::io;

namespace sgmnt{ namespace app{
    
    /*
     
     Extended Cinder AppNative Class.
     Setup general behavior I always using.
     And capable configured by settings xml files.
     
     */
    class AppBase : public ci::app::AppNative{
        
        public :
        
        AppBase(){
            ci::app::AppNative();
        }
        ~AppBase(){}
        
        // ===========================================================================
        // === PROPERTY. =============================================================
        
        // --- Common.
        
        XmlTree settingXml;
        
        gl::Texture backgroundTex;
        
        CameraPersp camera;
        
        struct tm * timeinfo;
        
        bool useOsc;
        bool useAudio;
        bool useCapture;
        bool doUpdateCapture;
        
        int nanoKontrolFader[16];
        
        sgmnt::io::OscInput     oscInput;
        sgmnt::io::AudioInput   audioInput;
        sgmnt::io::CaptureInput captureInput;
        
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
        
        virtual void setTextureToMap( string key, ci::gl::Texture tex );
        
        virtual void setTextureToMap( string key, string path );
        
        virtual ci::gl::Texture getTextureFromMap( string key );
        
        // ===========================================================================
        // === Utility ===============================================================
        
        virtual void updateTimeinfo();
        
        virtual string getTimeString(const char *format);
        
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
        
        //! OSC メッセージを受信した際の処理.
        void onReceiveOscMessage( sgmnt::io::OscInputEvent & event );
        
        // ===========================================================================
        // === For Debug. ============================================================
        
        /**
         * @virtual
         */
        virtual void drawFPS( Vec2f position = Vec2f(20.0f,20.0f) );
        
        protected :
        
        // ===========================================================================
        // === Member. ===============================================================
        
        string  mRemoteUrl;
        
        Vec2i   mWindowSize;
        
        Color   mColor;
        
        map<string,ci::gl::Texture> mTextureMap;
        
        // ===========================================================================
        // === Method. ===============================================================
        
        virtual void setupOsc();
        
    };
    
}}