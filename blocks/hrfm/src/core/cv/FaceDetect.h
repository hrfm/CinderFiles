#pragma once

#include "cinder/app/App.h"
#include "CinderOpenCV.h"
#include "DetectRect.h"
#include "XmlLoader.h"

using namespace std;
using namespace ci;

namespace hrfm{ namespace cv{
    
    class FaceDetect{
        
    public:
        
        FaceDetect( ivec2 textureSize = ivec2( 320, 240 ), int calcScale = 3 );
        ~FaceDetect();
        
        void update( Surface8uRef surface );
        void setCalcScale( int scale );
        vector<DetectRect> getFaces();
        
    private:
        
        void _updateFaces();
        
        pthread_mutex_t _mutex;
        
        int _calcScale;
        
        std::thread mThread;
        double recentSec;
        Surface8uRef mCloneSurface = NULL;
        ::cv::CascadeClassifier	mFaceCascade, mEyeCascade;
        vector<DetectRect>      mFaces, mEyes;
        
    };
    
}}