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
        
        FaceDetect( Vec2i textureSize = Vec2i( 320, 240 ), int calcScale = 3 );
        ~FaceDetect();
        
        void update( Surface surface );
        void setCalcScale( int scale );
        vector<DetectRect> getFaces();
        
    private:
        
        void _updateFaces();
        
        pthread_mutex_t _mutex;
        
        int _calcScale;
        
        std::thread mThread;
        double recentSec;
        Surface mCloneSurface;
        ::cv::CascadeClassifier	mFaceCascade, mEyeCascade;
        vector<DetectRect>      mFaces, mEyes;
        
    };
    
}}