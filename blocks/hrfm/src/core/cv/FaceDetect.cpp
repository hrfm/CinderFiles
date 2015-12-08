#include "FaceDetect.h"

using namespace std;
using namespace ci;

namespace hrfm{ namespace cv{
    
    // ========================================================================================= //
    
    void FaceDetect::setup( fs::path haarcascade_face, fs::path haarcascade_eye, ivec2 textureSize, int calcScale ){
        
        cout << "- hrfm::cv::FaceDetect::setup()" << endl << endl;
        
        _calcScale = calcScale;
        
        // ------------------------------------------------------------------
        
        const string facePathStr = hrfm::io::DataLoader::resolvePath(haarcascade_face).string();
        const string eyePathStr  = hrfm::io::DataLoader::resolvePath(haarcascade_eye).string();
        
        cout << "haarcascade_face xml : " << facePathStr <<  endl;
        if( !mFaceCascade.load( facePathStr ) ){
            cout << "[ERROR] FaceDetect::setup haarcascade_face load error." << endl;
        }
        cout << "haarcascade_eye  xml : " << eyePathStr <<  endl;
        if( !mEyeCascade.load( eyePathStr ) ){
            cout << "[ERROR] FaceDetect::setup haarcascade_eye load error." << endl;
        }
        
        // ------------------------------------------------------------------
        
        recentSec = 0;
        mThread = thread( bind( &FaceDetect::_updateFaces, this ) );
        
        cout << "--- FaceDetect::setup done. ---" << endl << endl;
        
    }
    
    void FaceDetect::update( Surface8uRef surface ){
        //cout << "FaceDetect::update()" << endl;
        if( pthread_mutex_lock(&_mutex) != 0 ){
            if( surface != NULL ){
                mCloneSurface = surface;
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
            
            if( mCloneSurface != NULL && 0.1f < elapsedSec - recentSec ){
                
                if( pthread_mutex_lock(&_mutex) != 0 ){
                    
                    try{
                        
                        const int calcScale = _calcScale; // calculate the image at half scale
                        
                        //*
                        
                        // create a grayscale copy of the input image
                        ::cv::Mat grayCameraImage( toOcv( *mCloneSurface, CV_8UC1 ) );
                        
                        // scale it to half size, as dictated by the calcScale constant
                        int scaledWidth  = mCloneSurface->getWidth() / calcScale;
                        int scaledHeight = mCloneSurface->getHeight() / calcScale;
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