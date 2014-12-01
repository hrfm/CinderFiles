#include "FaceDetect.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace hrfm{ namespace cv{
    
    // ========================================================================================= //
    
    FaceDetect::FaceDetect(){
        
    }
    
    FaceDetect::FaceDetect( Vec2i textureSize ){
        
        cout << endl << "- Setup FaceDetect." << endl;
        
        // --- Setup OpenCV
        
        //*
        try{
            
            mFaceCascade.load( getAssetPath( "haarcascade_frontalface_alt.xml" ).string() );
            mEyeCascade.load( getAssetPath( "haarcascade_eye.xml" ).string() );
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
            if( mFaceDetectEnabled && &surface != nullptr ){
                //mCloneSurface = surface.clone();
            }
            pthread_mutex_unlock(&_mutex);
        }
    }
    
    void FaceDetect::enable(){
        mFaceDetectEnabled = true;
    }
    
    void FaceDetect::disable(){
        mFaceDetectEnabled = false;
    }
    
    bool FaceDetect::isEnable(){
        return mFaceDetectEnabled;
    }
    
    vector<DetectRect> FaceDetect::getFaces(){
        return mFaces;
    }
    
    //! private
    
    void FaceDetect::_updateFaces(){
        
        ThreadSetup threadSetup;
        
        while(true){
            
            if( mFaceDetectEnabled == false ){
                continue;
            }
            
            double elapsedSec = getElapsedSeconds();
            
            if( mCloneSurface && 0.2f < elapsedSec - recentSec ){
                
                if( pthread_mutex_lock(&_mutex) != 0 ){
                    
                    try{
                        
                        const int calcScale = 3; // calculate the image at half scale
                        
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