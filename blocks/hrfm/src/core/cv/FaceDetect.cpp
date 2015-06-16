#include "FaceDetect.h"

using namespace std;
using namespace ci;

namespace hrfm{ namespace cv{
    
    // ========================================================================================= //
    
    FaceDetect::FaceDetect( Vec2i textureSize, int calcScale ){
        
        cout << endl << "- Setup FaceDetect." << endl;
        
        _calcScale = calcScale;
        
        // --- Setup OpenCV
        
        //*
        try{
            mFaceCascade.load( ci::app::getAssetPath( "haarcascade_frontalface_alt.xml" ).string() );
            mEyeCascade.load( ci::app::getAssetPath( "haarcascade_eye.xml" ).string() );
        }catch(...){
            fs::path path;
            path = getDocumentsDirectory() / "../Desktop" / "haarcascade_frontalface_alt.xml";
            mFaceCascade.load( path.string() );
            path = getDocumentsDirectory() / "../Desktop" / "haarcascade_eye.xml";
            mEyeCascade.load( path.string() );
        }
        //*/
        
        mThread = thread( bind( &FaceDetect::_updateFaces, this ) );
        recentSec = 0;
        
        cout << "done." << endl << endl;
        
    }
    
    void FaceDetect::update( Surface surface ){
        if( pthread_mutex_lock(&_mutex) != 0 ){
            if( &surface != nullptr ){
                mCloneSurface = surface.clone();
            }
            pthread_mutex_unlock(&_mutex);
        }
    }
    
    void FaceDetect::setCalcScale( int scale ){
        if( scale <= 0 ){
            scale = 1;
        }
        _calcScale = scale;
    }
    
    vector<DetectRect> FaceDetect::getFaces(){
        return mFaces;
    }
    
    //! private
    
    void FaceDetect::_updateFaces(){
        
        ThreadSetup threadSetup;
        
        while(true){
            
            double elapsedSec = ci::app::getElapsedSeconds();
            
            if( mCloneSurface && 0.1f < elapsedSec - recentSec ){
                
                if( pthread_mutex_lock(&_mutex) != 0 ){
                    
                    try{
                        
                        const int calcScale = _calcScale; // calculate the image at half scale
                        
                        // create a grayscale copy of the input image
                        ::cv::Mat grayCameraImage( toOcv( mCloneSurface, CV_8UC1 ) );
                        
                        //*
                        // scale it to half size, as dictated by the calcScale constant
                        int scaledWidth = mCloneSurface.getWidth() / calcScale;
                        int scaledHeight = mCloneSurface.getHeight() / calcScale;
                        ::cv::Mat smallImg( scaledHeight, scaledWidth, CV_8UC1 );
                        ::cv::resize( grayCameraImage, smallImg, smallImg.size(), 0, 0, ::cv::INTER_LINEAR );
                        
                        // equalize the histogram
                        ::cv::equalizeHist( smallImg, smallImg );
                        
                        // detect the faces and iterate them, appending them to mFaces
                        vector<::cv::Rect> faces;
                        mFaceCascade.detectMultiScale( smallImg, faces );
                        
                        for( vector<::cv::Rect>::const_iterator faceIter = faces.begin(); faceIter != faces.end(); ++faceIter ) {
                            
                            Rectf faceRect( fromOcv( *faceIter ) );
                            faceRect *= calcScale;
                            
                            // 既にある DetectRect と被っているかを調べ被っていればそれを更新する.
                            bool updated = false;
                            for( vector<DetectRect>::iterator it = mFaces.begin(); it != mFaces.end(); ++it ){
                                if( (*it).check( faceRect ) ){
                                    updated = true;
                                    break;
                                }
                            }
                            if( !updated ){
                                mFaces.push_back( DetectRect( faceRect ) );
                            }
                            
                            // detect eyes within this face and iterate them, appending them to mEyes
                            if( false ){
                                mEyes.clear();
                                vector<::cv::Rect> eyes;
                                mEyeCascade.detectMultiScale( smallImg( *faceIter ), eyes );
                                for( vector<::cv::Rect>::const_iterator eyeIter = eyes.begin(); eyeIter != eyes.end(); ++eyeIter ) {
                                    Rectf eyeRect( fromOcv( *eyeIter ) );
                                    eyeRect = eyeRect * calcScale + faceRect.getUpperLeft();
                                    mEyes.push_back( eyeRect );
                                }
                            }
                            
                        }
                        
                        vector<DetectRect>::iterator it = mFaces.begin();
                        while( it != mFaces.end() ){
                            (*it).update();
                            if( (*it).dead() ){
                                it = mFaces.erase(it);
                            }else{
                                it++;
                            }
                        }
                        
                        //*/
                        
                    }catch(...){}
                    
                    recentSec = elapsedSec;
                    
                    pthread_mutex_unlock(&_mutex);
                    
                }
                
            }
            
            
        }
        
    }
    
    
}}