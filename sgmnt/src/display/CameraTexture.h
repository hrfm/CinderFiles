#pragma once

#include "IDrawable.h"
#include "EventDispatcher.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Capture.h"

namespace sgmnt{ namespace display{
    
    /*
     
     ci::Capture を用いてカメラの内容を表示するクラスです.
     Capture の更新と Texture への描画のみを行うシンプルなクラスです.
     
     FaceDetect 等を行う場合はこのクラスを継承するか Delegate すると便利です.
     
     */
    class CameraTexture : public IDrawable, public sgmnt::events::EventDispatcher{
        
    public:
        
        CameraTexture();
        CameraTexture( string deviceName );
        CameraTexture( int32_t width, int32_t height, const string deviceName );
        ~CameraTexture();
        
        virtual void init( int32_t width, int32_t height, const string deviceName = "*" );
        
        virtual void showAllDevices();
        
        virtual bool captureAvailable();
        
        virtual ci::CaptureRef  getCaptureRef();
        virtual ci::gl::Texture getTexture();
        
        virtual void update();
        
    protected:
        
        virtual void _draw();
        
    private:
        
        ci::CaptureRef  mCapture;
        bool            mCaptureAvailable;
        ci::gl::Texture mTexture;
        
    };
    
}}