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
    
    void CaptureInput::setup( int32_t width, int32_t height, string deviceName = "*" ){
        
        // --- Init Properties. ---
        
        mCaptureSize    = Vec2i( width, height );
        mCaptureReflect = false;
        
        // --- Setup Capture Devices. ---
        
        _deviceName = deviceName;
        
        SiCaptureInput::getInstance().showAllDevices();
        mCapture = SiCaptureInput::getInstance().createRef( width, height, _deviceName );
        mCaptureAvailable = true;
        
        mDrawBounds = mCapture->getBounds();
        
        // --- Create GLSL. ---
        
    }
    
    void CaptureInput::update(){
        if( mCapture && mCapture->checkNewFrame() ) {
            if( mFaceDetect ){
                mFaceDetect->update( mCapture->getSurface().clone() );
            }
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
        return mCapture->getBounds();
    }
    
    bool CaptureInput::isCaptureAvailable(){
        return mCaptureAvailable;
    }
    
    ci::gl::Texture CaptureInput::getCaptureTexture(){
        return SiCaptureInput::getInstance().getTexture( _deviceName );
    }
    
    ci::gl::Texture CaptureInput::getDiffTexture(){
        return SiCaptureInput::getInstance().getDiffTexture( _deviceName );
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
            mOpticalFlow->update( SiCaptureInput::getInstance().getTexture( _deviceName ), bias, frameRate );
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
    
    ci::gl::Texture CaptureInput::getOpticalFlowTexture(){
        return mOpticalFlow->getOpticalFlowTexture();
    }
    
    Vec2i CaptureInput::getOpticalFlowTextureSize(){
        return mOpticalFlow->getOpticalFlowTextureSize();
    }
    
    Rectf CaptureInput::getOpticalFlowTextureBounds(){
        return mOpticalFlow->getOpticalFlowTextureBounds();
    }
    
}}