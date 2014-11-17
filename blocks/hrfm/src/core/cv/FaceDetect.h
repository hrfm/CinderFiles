#pragma once

#include "cinder/app/AppBasic.h"
#include "CinderOpenCV.h"
#include "DetectRect.h"
#include "XmlLoader.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace hrfm{ namespace cv{
    
    class FaceDetect{
        
    public:
        
        FaceDetect( );
        FaceDetect( Vec2i textureSize );
        ~FaceDetect();
        
        void update( Surface surface );
        void enable();
        void disable();
        bool isEnable();
        vector<DetectRect> getFaces();
        
    private:
        
        void _updateFaces();
        
        bool mFaceDetectEnabled;
        
        double recentSec;
        
        std::thread mThread;
        
        Surface mCloneSurface;
        
        ::cv::CascadeClassifier	mFaceCascade, mEyeCascade;
        vector<DetectRect>      mFaces, mEyes;
        
    };
    
}}