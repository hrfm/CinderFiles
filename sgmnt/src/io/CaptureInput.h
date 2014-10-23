#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/Capture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"

#include "cinder/Xml.h"
#include "cinder/Utilities.h"

#include "CinderOpenCV.h"

#include "DetectRect.h"
#include "ForceMap.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace sgmnt::live_effects;

namespace sgmnt{ namespace io{
    
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
        
        gl::Texture getCaptureTexture();
        
        gl::Texture getDiffTexture();
        
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
        
        vector<cv::Point2f> prevFeatures, features;
        vector<uint8_t>		featureStatuses;
        
        void setupOpticalFlow( Vec2i textureSize );
        
        void updateOpticalFlow( float bias = 1.0, float frameRate = 30.0 );
        
        void drawOpticalFlow();
        
        ForceMap getForceMap();
        
        void drawForceMap();
        
        gl::Texture getOpticalFlowTexture();
        
        Vec2i getOpticalFlowTextureSize();
        
        Rectf getOpticalFlowTextureBounds();
        
    private:
        
        void updateFaces();
        
        void chooseFeatures( cv::Mat currentFrame );
        
        void trackFeatures( cv::Mat currentFrame );
        
        // --- Video Capture. ---
        
        CaptureRef              mCapture;
        gl::Texture             mTexture;
        
        Vec2i                   mCaptureSize;
        Rectf                   mDrawBounds;
        gl::Fbo                 mCaptureFBO;
        
        bool                    mCaptureAvailable;
        bool                    mCaptureReflect;
        
        gl::GlslProg            mDiffShader;
        gl::Fbo                 mCaptureDiffFBO;
        
        // ----- Face Detect.
        
        bool                    mFaceDetectSetuped;
        bool                    mFaceDetectEnabled;
        
        double                  recentSec;
        std::thread             mThread;
        Surface                 cloneSurface;
        
        cv::CascadeClassifier	mFaceCascade, mEyeCascade;
        vector<DetectRect>      mFaces, mEyes;
        
        float recentTime;
        
        // ----- Optical Flow.
        
        static const int        MAX_FEATURES = 256;
        
        gl::Fbo                 mOpticalFlowFBO;
        Rectf                   mOpticalFlowBounds;
        cv::Mat                 mPrevFrame;
        ForceMap                mForceMap;
        
    };
    
}}