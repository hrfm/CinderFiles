#include "CaptureInput.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace hrfm::io;
using namespace hrfm::cv;

namespace hrfm{ namespace io{
    
    CaptureInput::CaptureInput(){};
    
    void CaptureInput::setup( int32_t width, int32_t height ){
        setup( width, height, "*" );
    }
    
    void CaptureInput::setup( int32_t width, int32_t height, string deviceName ){
        
        // --- Init Properties. ---
        
        mCaptureSize    = Vec2i( width, height );
        mCaptureReflect = false;
        
        // --- Create Fbo. ---
        
        gl::Fbo::Format captureFormat;
        mCaptureFBO     = gl::Fbo( width, height, captureFormat );
        mCaptureDiffFBO = gl::Fbo( width, height, captureFormat );
        mDrawBounds     = mCaptureFBO.getBounds();
        
        // --- Setup Capture Devices. ---
        
        SiCaptureInput::getInstance().showAllDevices();
        mCapture = SiCaptureInput::getInstance().createRef( width, height, deviceName );
        mCaptureAvailable = true;
        
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
            
            if( mFaceDetect ){
                mFaceDetect->update( mCapture->getSurface().clone() );
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
        if( !mFaceDetect ){
            mFaceDetect = new FaceDetect( textureSize );
        }
        mFaceDetect->enable();
    }
    
    void CaptureInput::enableFaceDetect(){
        if( mFaceDetect ){
            mFaceDetect->enable();
        }
    }
    
    void CaptureInput::disableFaceDetect(){
        if( mFaceDetect ){
            mFaceDetect->disable();
        }
    }
    
    bool CaptureInput::faceDetectEnabled(){
        return mFaceDetect && mFaceDetect->isEnable();
    }
    
    vector<DetectRect> CaptureInput::getFaces(){
        return mFaceDetect->getFaces();
    }
    
    // ========================================================================================
    //
    // Optical Flow.
    //
    // ========================================================================================
    
    void CaptureInput::setupOpticalFlow( Vec2i textureSize ){
        if( mOpticalFlow ){
            mOpticalFlow = new OpticalFlow( textureSize );
        }
    }
    
    void CaptureInput::updateOpticalFlow( float bias, float frameRate ){
        if( mOpticalFlow ){
            mOpticalFlow->update( mCaptureFBO.getTexture(), bias, frameRate );
        }
    }
    
    void CaptureInput::drawOpticalFlow(){
        if( mOpticalFlow ){
            mOpticalFlow->draw();
        }
    }
    
    ForceMap CaptureInput::getForceMap(){
        return mOpticalFlow->getForceMap();
    }
    
    void CaptureInput::drawForceMap(){
        Vec2f texSize = Vec2f( getOpticalFlowTextureSize() );
        getForceMap().draw( texSize );
    }
    
    gl::Texture CaptureInput::getOpticalFlowTexture(){
        return mOpticalFlow->getOpticalFlowTexture();
    }
    
    Vec2i CaptureInput::getOpticalFlowTextureSize(){
        return mOpticalFlow->getOpticalFlowTextureSize();
    }
    
    Rectf CaptureInput::getOpticalFlowTextureBounds(){
        return mOpticalFlow->getOpticalFlowTextureBounds();
    }
    
}}