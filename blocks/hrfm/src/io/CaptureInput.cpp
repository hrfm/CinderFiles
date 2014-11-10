#include "CaptureInput.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace hrfm::io;
using namespace hrfm::live_effects;

namespace hrfm{ namespace io{
    
    CaptureInput::CaptureInput(){};
    
    void CaptureInput::setup( int32_t width, int32_t height ){
        setup( width, height, "*" );
    }
    
    void CaptureInput::setup( int32_t width, int32_t height, string deviceName ){
        
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
        
        SiCaptureInput::getInstance().showAllDevices();
        
        //*
        mCapture = SiCaptureInput::getInstance().createRef( width, height, deviceName );
        mCaptureAvailable = true;
        /*/
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
        //*/
        
        // --- Create GLSL. ---
        
        try {
            mDiffShader = gl::GlslProg( loadResource("../resources/simple_vert.glsl"), loadResource("../resources/diff_frag.glsl") );
        }catch( ci::gl::GlslProgCompileExc &exc ) {
            cout << "Shader compile error: " << endl;
            cout << exc.what();
        }catch( ... ) {
            cout << "Unable to load shader" << endl;
        }
        
    }
    
    void CaptureInput::update(){
        
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
    
    bool CaptureInput::captureAvailable(){
        return mCaptureAvailable;
    }
    
    void CaptureInput::enableCaptureReflect(){
        if( mCaptureReflect == false ){
            mCaptureReflect = true;
            mDrawBounds.set(mDrawBounds.x2,mDrawBounds.y1,mDrawBounds.x1,mDrawBounds.y2);
        }
    }
    void CaptureInput::disableCaptureReflect(){
        if( mCaptureReflect == true ){
            mCaptureReflect = false;
            mDrawBounds.set(mDrawBounds.x2,mDrawBounds.y1,mDrawBounds.x1,mDrawBounds.y2);
        }
    }
    
    Vec2i CaptureInput::getSize(){
        return mCaptureSize;
    }
    
    Rectf CaptureInput::getBounds(){
        return mCaptureFBO.getBounds();
    }
    
    bool CaptureInput::isCaptureAvailable(){
        return mCaptureAvailable;
    }
    
    gl::Texture CaptureInput::getCaptureTexture(){
        return mCaptureFBO.getTexture();
    }
    
    gl::Texture CaptureInput::getDiffTexture(){
        return mCaptureDiffFBO.getTexture();
    }
    
    void CaptureInput::bindTexture( int index ){
        mCaptureFBO.bindTexture( index );
    }
    
    void CaptureInput::unbindTexture(){
        mCaptureFBO.unbindTexture();
    }
    
    void CaptureInput::showAllDevices(){
        // print the devices
        for( auto device = Capture::getDevices().begin(); device != Capture::getDevices().end(); ++device ) {
            cout << "Device: " << (*device)->getName() << " " << std::endl;
        }
    }
    
    void CaptureInput::quit(){
        mCapture.reset();
    }
    
    
    
    // ========================================================================================
    //
    // FaceDetect.
    //
    // ========================================================================================
    
    void CaptureInput::setupFaceDetect(){
        setupFaceDetect( Vec2i( 480, 270 ) );
    }
    
    void CaptureInput::setupFaceDetect( Vec2i textureSize ){
        
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
    
    void CaptureInput::enableFaceDetect(){
        if( mFaceDetectSetuped == false ){
            return;
        }
        mFaceDetectEnabled = true;
    }
    
    void CaptureInput::disableFaceDetect(){
        mFaceDetectEnabled = false;
    }
    
    bool CaptureInput::faceDetectEnabled(){
        return mFaceDetectEnabled;
    }
    
    vector<DetectRect> CaptureInput::getFaces(){
        return mFaces;
    }
    
    // ========================================================================================
    //
    // Optical Flow.
    //
    // ========================================================================================
    
    void CaptureInput::setupOpticalFlow( Vec2i textureSize ){
        gl::Fbo::Format captureFormat;
        mOpticalFlowFBO    = gl::Fbo( textureSize.x, textureSize.y, captureFormat );
        mOpticalFlowBounds = mOpticalFlowFBO.getBounds();
    }
    
    void CaptureInput::updateOpticalFlow( float bias, float frameRate ){
        
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
    
    void CaptureInput::drawOpticalFlow(){
        
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
    
    ForceMap CaptureInput::getForceMap(){
        return mForceMap;
    }
    
    void CaptureInput::drawForceMap(){
        Vec2f texSize = Vec2f( getOpticalFlowTextureSize() );
        mForceMap.draw( texSize );
    }
    
    gl::Texture CaptureInput::getOpticalFlowTexture(){
        return mOpticalFlowFBO.getTexture();
    }
    
    Vec2i CaptureInput::getOpticalFlowTextureSize(){
        return mOpticalFlowFBO.getSize();
    }
    
    Rectf CaptureInput::getOpticalFlowTextureBounds(){
        return mOpticalFlowFBO.getBounds();
    }
    
    void CaptureInput::updateFaces(){
        
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
    
    void CaptureInput::chooseFeatures( cv::Mat currentFrame ){
        cv::goodFeaturesToTrack( currentFrame, features, MAX_FEATURES, 0.005, 3.0 );
    }
    
    void CaptureInput::trackFeatures( cv::Mat currentFrame ){
        vector<float> errors;
        prevFeatures = features;
        if( ! features.empty() ){
            cv::calcOpticalFlowPyrLK( mPrevFrame, currentFrame, prevFeatures, features, featureStatuses, errors );
        }
    }
    
}}