#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/Capture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"

#include "cinder/Xml.h"
#include "cinder/Utilities.h"

#include "CinderOpenCV.h"
#include "SiCaptureInput.h"

#include "FaceDetect.h"
#include "OpticalFlow.h"

using namespace ci;
using namespace std;

using namespace hrfm::cv;

namespace hrfm{ namespace io{
    
    class CaptureInput{
        
    public:
        
        CaptureInput();
        
        void setup( int32_t width, int32_t height, string deviceName = "*", int cacheLength = 2 );
        
        void update();
        
        Vec2i getSize();
        Rectf getBounds();
        
        bool isCaptureAvailable();
        
        ci::gl::Texture getCaptureTexture();
        ci::gl::Texture getDiffTexture();
        
        //void bindTexture( int index );
        //void unbindTexture();
        
        void quit();
        
        // ========================================================================================
        //
        // FaceDetect.
        //
        // ========================================================================================
        
        void setupFaceDetect( Vec2i textureSize = Vec2i(480,270) );
        bool faceDetectEnabled();
        vector<DetectRect> getFaces();
        
    private:
        
        // --- Video Capture. ---
        
        string     _deviceName;
        CaptureRef mCapture;
        
        Vec2i mCaptureSize;
        Rectf mDrawBounds;
        
        bool mCaptureAvailable;
        bool mCaptureReflect;
        
        FaceDetect  * mFaceDetect  = NULL;
        
    };
    
}}