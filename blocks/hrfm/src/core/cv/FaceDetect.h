#pragma once

#include "cinder/app/App.h"
#include "DetectRect.h"
#include "XmlLoader.h"
#include "CinderOpenCV.h"

using namespace std;
using namespace ci;

namespace hrfm{ namespace cv{
    
    class FaceDetect{
        
    public:
        
        FaceDetect(){};
        ~FaceDetect();
        
        void setup(
            ci::fs::path haarcascade_face,
            ci::fs::path haarcascade_eye,
            ivec2 textureSize = ivec2( 320, 240 ),
            int calcScale = 3
        );
        
        void setCalcScale( int scale );
        
        void update( Surface8uRef surface );
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