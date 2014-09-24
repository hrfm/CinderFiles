#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/Capture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"

#include "cinder/Xml.h"
#include "cinder/Utilities.h"

#include "CinderOpenCV.h"

#include "ForceMap.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace sgmnt::live_effects;

namespace sgmnt{ namespace io{
    
    // ========================================================================================= //
    
    class DetectRect : public Rectf{
        
        public :
        
            DetectRect( float x1, float y1, float x2, float y2 ){
                setup( Rectf(x1, y1, x2, y2) );
            }
            
            DetectRect( Rectf rect ){
                setup(rect);
            }
        
            void update(){
                if( 0 < life ){ life--; }
                x1 += ( targetRect.x1 - x1 ) * 0.98;
                y1 += ( targetRect.y1 - y1 ) * 0.98;
                x2 += ( targetRect.x2 - x2 ) * 0.98;
                y2 += ( targetRect.y2 - y2 ) * 0.98;
                time++;
            }
        
            bool check( Rectf rect ){
                // 距離が一定以内の場合は同一の顔とみなす.
                if( rect.distance( getCenter() ) < 10 ){
                    targetRect.set( rect.x1, rect.y1, rect.x2, rect.y2 );
                    life = lifeMax;
                    return true;
                }
                return false;
            }
        
            bool dead(){
                if( life <= 0 ){
                    //cout << "dead!!" << endl;
                    return life <= 0;
                }else{
                    return false;
                }
            }
        
            int time;
        
        private :
        
            void setup( Rectf rect, int lifespan = 12 ){
                set( rect.x1, rect.y1, rect.x2, rect.y2 );
                targetRect.set( rect.x1, rect.y1, rect.x2, rect.y2 );
                life    = lifespan;
                lifeMax = lifespan;
                time    = 0;
            }
            
            int life;
            int lifeMax;
            Rectf targetRect;
        
    };
    
    class CaptureInput{
        
        public:
            
            CaptureInput(){};
        
            bool useOpticalFlow;
            
            void setup( int32_t width, int32_t height ){
                setup( width, height, "*" );
            }
            
            void setup( int32_t width, int32_t height, string deviceName ){
                
                // --- Init Properties. ---
                
                mCaptureSize    = Vec2i( width, height );
                mCaptureReflect = false;
                
                mFaceDetectEnabled = false;
                mFaceDetectSetuped = false;
                
                // --- Create ForceMap. ---
                
                mForceMap.setup( 16 * 4, 9 * 4 );
                
                // --- Create Fbo. ---
                
                gl::Fbo::Format captureFormat;
                mCaptureFBO     = gl::Fbo( width, height, captureFormat );
                mCaptureDiffFBO = gl::Fbo( width, height, captureFormat );
                mDrawBounds     = mCaptureFBO.getBounds();
                
                // --- Setup Capture Devices. ---
                
                showAllDevices();
                
                try {
                    if( deviceName != "*" ){
                        mCapture = Capture::create( width, height, Capture::findDeviceByName(deviceName) );
                    }else{
                        mCapture = Capture::create( width, height );
                    }
                    mCapture->start();
                    mCaptureAvailable = true;
                }catch( ... ) {
                    cout << "Failed to initialize capture" << std::endl;
                    mCaptureAvailable = false;
                }
                
                // --- Create GLSL. ---
                
                try {
                    mDiffShader = gl::GlslProg( loadResource("../resources/simple_vert.glsl"), loadResource("../resources/diff_frag.glsl") );
                }catch( ci::gl::GlslProgCompileExc &exc ) {
                    cout << "Shader compile error: " << endl;
                    cout << exc.what();
                }catch( ... ) {
                    cout << "Unable to load shader" << endl;
                }
                
                cout << "fuck" << endl;
            }
            
            void update(){
                
                if( mCapture && mCapture->checkNewFrame() ) {
                    
                    if( mFaceDetectEnabled ){
                        cloneSurface = mCapture->getSurface().clone();
                    }
                    
                    mTexture = gl::Texture( mCapture->getSurface() );
                    
                    Area viewport = gl::getViewport();
                    
                    // ---
                    
                    // 前フレームとの差分を取得する.
                    mCaptureDiffFBO.bindFramebuffer();
                        gl::pushMatrices();
                            gl::setViewport( mCaptureDiffFBO.getBounds() );
                            gl::setMatricesWindow( mCaptureDiffFBO.getSize(), false );
                            gl::clear( Color( 0, 0, 0 ) );
                            gl::color(1.0f,1.0f,1.0f);
                            glPushMatrix();
                                gl::draw( mTexture, mDrawBounds );
                            glPopMatrix();
                            mCaptureDiffFBO.bindTexture(0);
                            mCaptureFBO.bindTexture(1);
                            mDiffShader.bind();
                                mDiffShader.uniform("tex0", 0);
                                mDiffShader.uniform("tex1", 1);
                                mDiffShader.uniform("resolution", Vec2f( mCaptureDiffFBO.getWidth(), mCaptureDiffFBO.getHeight() ) );
                                gl::drawSolidRect( mDrawBounds );
                            mDiffShader.unbind();
                            mCaptureFBO.unbindTexture();
                            mCaptureDiffFBO.unbindTexture();
                        gl::popMatrices();
                    mCaptureDiffFBO.unbindFramebuffer();
                    
                    // --- キャプチャ用 Frame Buffer の更新.
                    
                    mCaptureFBO.bindFramebuffer();
                        gl::pushMatrices();
                            gl::setViewport( mCaptureFBO.getBounds() );
                            gl::setMatricesWindow( mCaptureFBO.getSize(), false );
                            gl::clear( Color( 0, 0, 0 ) );
                            gl::color(1.0f,1.0f,1.0f);
                            glPushMatrix();
                                gl::draw( mTexture, mDrawBounds );
                            glPopMatrix();
                        gl::popMatrices();
                    mCaptureFBO.unbindFramebuffer();
                    
                    gl::setViewport( viewport );
                    
                }
                
                // clear out the previously deteced faces & eyes
                for( vector<DetectRect>::iterator it = mFaces.begin(); it != mFaces.end(); ++it ){
                    (*it).update();
                }
                
            }
        
            bool captureAvailable(){
                return mCaptureAvailable;
            }
            
            void enableCaptureReflect(){
                if( mCaptureReflect == false ){
                    mCaptureReflect = true;
                    mDrawBounds.set(mDrawBounds.x2,mDrawBounds.y1,mDrawBounds.x1,mDrawBounds.y2);
                }
            }
            void disableCaptureReflect(){
                if( mCaptureReflect == true ){
                    mCaptureReflect = false;
                    mDrawBounds.set(mDrawBounds.x2,mDrawBounds.y1,mDrawBounds.x1,mDrawBounds.y2);
                }
            }
        
            Vec2i getSize(){
                return mCaptureSize;
            }
        
            Rectf getBounds(){
                return mCaptureFBO.getBounds();
            }
        
            bool isCaptureAvailable(){
                return mCaptureAvailable;
            }
            
            gl::Texture getCaptureTexture(){
                return mCaptureFBO.getTexture();
            }
            
            gl::Texture getDiffTexture(){
                return mCaptureDiffFBO.getTexture();
            }
            
            void bindTexture( int index ){
                mCaptureFBO.bindTexture( index );
            }
            
            void unbindTexture(){
                mCaptureFBO.unbindTexture();
            }
            
            void showAllDevices(){
                // print the devices
                for( auto device = Capture::getDevices().begin(); device != Capture::getDevices().end(); ++device ) {
                    cout << "Device: " << (*device)->getName() << " " << std::endl;
                }
            }
            
            void quit(){
                mCapture.reset();
            }
        
        
        
            // ========================================================================================
            //
            // FaceDetect.
            //
            // ========================================================================================
        
            void setupFaceDetect(){
                setupFaceDetect( Vec2i( 480, 270 ) );
            }
        
            void setupFaceDetect( Vec2i textureSize ){
                
                if( mFaceDetectSetuped == true ){
                    return;
                }
                
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
                
                mThread = thread( bind( &CaptureInput::updateFaces, this ) );
                recentSec = 0;
                
                mFaceDetectSetuped = true;
                
                enableFaceDetect();
                
                cout << endl;
                
            }
        
            void enableFaceDetect(){
                if( mFaceDetectSetuped == false ){
                    return;
                }
                mFaceDetectEnabled = true;
            }
            
            void disableFaceDetect(){
                mFaceDetectEnabled = false;
            }
        
            bool faceDetectEnabled(){
                return mFaceDetectEnabled;
            }
        
            vector<DetectRect> getFaces(){
                return mFaces;
            }
        
            // ========================================================================================
            //
            // Optical Flow.
            //
            // ========================================================================================
        
            vector<cv::Point2f> prevFeatures, features;
            vector<uint8_t>		featureStatuses;
            
            void setupOpticalFlow( Vec2i textureSize ){
                gl::Fbo::Format captureFormat;
                mOpticalFlowFBO    = gl::Fbo( textureSize.x, textureSize.y, captureFormat );
                mOpticalFlowBounds = mOpticalFlowFBO.getBounds();
            }
            
            void updateOpticalFlow( float bias = 1.0, float frameRate = 30.0 ){
                
                gl::Texture texture = mCaptureFBO.getTexture();
                
                mOpticalFlowFBO.bindFramebuffer();
                    gl::pushMatrices();
                    gl::setMatricesWindow( getWindowSize(), false );
                    gl::clear();
                    gl::color(1.0f,1.0f,1.0f);
                    glPushMatrix();
                        gl::draw( texture, mOpticalFlowBounds );
                    glPopMatrix();
                    gl::popMatrices();
                mOpticalFlowFBO.unbindFramebuffer();
                
                if( 1.0 / frameRate < getElapsedSeconds() - recentTime ){
                    
                    cv::Mat currentFrame( toOcv( Channel( Surface( mOpticalFlowFBO.getTexture() ) ) ) );
                    
                    if( mPrevFrame.data ) {
                        if( features.empty() || getElapsedFrames() % 30 == 0 ){
                            // pick new features once every 30 frames, or the first frame
                            chooseFeatures( mPrevFrame );
                        }
                        trackFeatures( currentFrame );
                    }
                    
                    mPrevFrame = currentFrame;
                    
                    // --- Update ForceMap. ---
                    
                    Vec2f from, to, size( mOpticalFlowFBO.getSize() );
                    float dist;
                    
                    int max = features.size();
                    for( size_t idx = 0; idx < max; ++idx ) {
                        if( featureStatuses[idx] ) {
                            from = fromOcv( features[idx] );
                            to   = fromOcv( prevFeatures[idx] );
                            dist = from.distance(to);
                            if( 10.f < dist && dist < 50.0f ){
                                mForceMap.addForce( from / size, ( to - from ) * bias );
                            }
                        }
                    }
                    
                    recentTime = getElapsedSeconds();

                }
                
                mForceMap.update();
                
            }
        
            void drawOpticalFlow(){
                
                Vec2f from, to;
                float dist;
                int length = features.size();
                
                gl::lineWidth(2.0);
                glDisable( GL_TEXTURE_2D );
                
                glColor4f( 1, 1, 0, 0.5f );
                // draw all the old points
                for( vector<cv::Point2f>::const_iterator featureIt = prevFeatures.begin(); featureIt != prevFeatures.end(); ++featureIt ){
                    gl::drawStrokedCircle( fromOcv( *featureIt ), 1 );
                }
                
                // draw all the new points
                for( vector<cv::Point2f>::const_iterator featureIt = features.begin(); featureIt != features.end(); ++featureIt ){
                    gl::drawSolidCircle( fromOcv( *featureIt ), 1 );
                }
                
                // draw the lines connecting them
                glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
                
                glBegin( GL_LINES );
                
                    for( size_t idx = 0; idx < length; ++idx ) {
                        if( featureStatuses[idx] ) {
                            from = fromOcv( features[idx] );
                            to   = fromOcv( prevFeatures[idx] );
                            dist = from.distance(to);
                            if( dist < 40.0 ){
                                gl::vertex( from );
                                gl::vertex( to );
                            }
                        }
                    }
                
                glEnd();
                
            }
        
            ForceMap getForceMap(){
                return mForceMap;
            }
        
            void drawForceMap(){
                Vec2f texSize = Vec2f( getOpticalFlowTextureSize() );
                mForceMap.draw( texSize );
            }
        
            gl::Texture getOpticalFlowTexture(){
                return mOpticalFlowFBO.getTexture();
            }
            
            Vec2i getOpticalFlowTextureSize(){
                return mOpticalFlowFBO.getSize();
            }
            
            Rectf getOpticalFlowTextureBounds(){
                return mOpticalFlowFBO.getBounds();
            }
        
        
        
        protected :
        
        
        
        private:
            
            void updateFaces(){
                
                ThreadSetup threadSetup;
                
                while(true){
                    
                    if( mFaceDetectEnabled == false ){
                        continue;
                    }
                    
                    double elapsedSec = getElapsedSeconds();
                    
                    if( cloneSurface ){
                        
                        try{
                            
                            const int calcScale = 3; // calculate the image at half scale
                            
                            // create a grayscale copy of the input image
                            cv::Mat grayCameraImage( toOcv( cloneSurface, CV_8UC1 ) );
                            
                            //*
                            // scale it to half size, as dictated by the calcScale constant
                            int scaledWidth = cloneSurface.getWidth() / calcScale;
                            int scaledHeight = cloneSurface.getHeight() / calcScale;
                            cv::Mat smallImg( scaledHeight, scaledWidth, CV_8UC1 );
                            cv::resize( grayCameraImage, smallImg, smallImg.size(), 0, 0, cv::INTER_LINEAR );
                            
                            // equalize the histogram
                            cv::equalizeHist( smallImg, smallImg );
                            
                            // detect the faces and iterate them, appending them to mFaces
                            vector<cv::Rect> faces;
                            mFaceCascade.detectMultiScale( smallImg, faces );
                            
                            for( vector<cv::Rect>::const_iterator faceIter = faces.begin(); faceIter != faces.end(); ++faceIter ) {
                                
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
                                //vector<cv::Rect> eyes;
                                //mEyeCascade.detectMultiScale( smallImg( *faceIter ), eyes );
                                //for( vector<cv::Rect>::const_iterator eyeIter = eyes.begin(); eyeIter != eyes.end(); ++eyeIter ) {
                                //    Rectf eyeRect( fromOcv( *eyeIter ) );
                                //    eyeRect = eyeRect * calcScale + faceRect.getUpperLeft();
                                //    mEyes.push_back( eyeRect );
                                //}
                                
                            }
                            
                            vector<DetectRect>::iterator it = mFaces.begin();
                            while( it != mFaces.end() ){
                                if( (*it).dead() ){
                                    it = mFaces.erase(it);
                                }else{
                                    it++;
                                }
                            }
                            
                            //*/
                            
                        }catch(...){}
                        
                        recentSec = elapsedSec;
                        
                    }
                    
                }
                
            }
            
            void chooseFeatures( cv::Mat currentFrame ){
                cv::goodFeaturesToTrack( currentFrame, features, MAX_FEATURES, 0.005, 3.0 );
            }
            
            void trackFeatures( cv::Mat currentFrame ){
                vector<float> errors;
                prevFeatures = features;
                if( ! features.empty() ){
                    cv::calcOpticalFlowPyrLK( mPrevFrame, currentFrame, prevFeatures, features, featureStatuses, errors );
                }
            }
            
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