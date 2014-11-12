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
using namespace ci::app;
using namespace std;

using namespace hrfm::cv;

namespace hrfm{ namespace io{
    
    class CaptureInput{
        
    public:
        
        CaptureInput();
        
        bool useOpticalFlow;
        
        void setup( int32_t width, int32_t height );
        void setup( int32_t width, int32_t height, string deviceName );
        
        void update();
        
        bool captureAvailable();
        
        void enableCaptureReflect();
        void disableCaptureReflect();
        
        Vec2i getSize();
        
        Rectf getBounds();
        
        bool isCaptureAvailable();
        
        ci::gl::Texture getCaptureTexture();
        
        ci::gl::Texture getDiffTexture();
        
        void bindTexture( int index );
        
        void unbindTexture();
        
        void showAllDevices();
        
        void quit();
        
        // ========================================================================================
        //
        // FaceDetect.
        //
        // ========================================================================================
        
        void setupFaceDetect();
        void setupFaceDetect( Vec2i textureSize );
        void enableFaceDetect();
        void disableFaceDetect();
        bool faceDetectEnabled();
        vector<DetectRect> getFaces();
        
        // ========================================================================================
        //
        // Optical Flow.
        //
        // ========================================================================================
        
        void setupOpticalFlow( Vec2i textureSize );
        void updateOpticalFlow( float bias = 1.0, float frameRate = 30.0 );
        void drawOpticalFlow();
        ForceMap getForceMap();
        void drawForceMap();
        ci::gl::Texture getOpticalFlowTexture();
        Vec2i getOpticalFlowTextureSize();
        Rectf getOpticalFlowTextureBounds();
        
    private:
        
        // --- Video Capture. ---
        
        CaptureRef              mCapture;
        ci::gl::Texture             mTexture;
        
        Vec2i                   mCaptureSize;
        Rectf                   mDrawBounds;
        ci::gl::Fbo                 mCaptureFBO;
        
        bool                    mCaptureAvailable;
        bool                    mCaptureReflect;
        
        ci::gl::GlslProg            mDiffShader;
        ci::gl::Fbo                 mCaptureDiffFBO;
        
        // ----- Face Detect.
        
        FaceDetect * mFaceDetect;
        
        // ----- Optical Flow.
        
        OpticalFlow * mOpticalFlow;
        
    };
    
}}