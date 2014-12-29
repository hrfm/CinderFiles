#include "CaptureInput.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace hrfm::io;
using namespace hrfm::cv;

namespace hrfm{ namespace io{
    
    CaptureInput::CaptureInput(){};
    
    void CaptureInput::setup( int32_t width, int32_t height, string deviceName, int cacheLength ){
        // --- Setup Capture Devices. ---
        _deviceName = deviceName;
        SiCaptureInput::getInstance().showAllDevices();
        SiCaptureInput::getInstance().setDefaultDeviceName( deviceName );
        mCapture = SiCaptureInput::getInstance().createRef( width, height, _deviceName, cacheLength );
        // --- Create GLSL. ---
    }
    
    void CaptureInput::update(){
        //if( mCapture && mCapture->checkNewFrame() ){
            if( mFaceDetect ){
                mFaceDetect->update( mCapture->getSurface().clone() );
            }
            if( mOpticalFlow ){
                updateOpticalFlow();
            }
        //}
    }
    
    Vec2i CaptureInput::getSize(){
        return mCapture->getSize();
    }
    
    Rectf CaptureInput::getBounds(){
        return mCapture->getBounds();
    }
    
    ci::gl::Texture CaptureInput::getCaptureTexture(){
        return SiCaptureInput::getInstance().getTexture( _deviceName );
    }
    
    ci::gl::Texture CaptureInput::getDiffTexture(){
        return SiCaptureInput::getInstance().getDiffTexture( _deviceName );
    }
    
    void CaptureInput::quit(){
        mCapture.reset();
    }
    
    // ========================================================================================
    //
    // FaceDetect.
    //
    // ========================================================================================
    
    void CaptureInput::setupFaceDetect( Vec2i textureSize ){
        if( mFaceDetect == NULL ){
            mFaceDetect = new FaceDetect( textureSize );
        }
    }
    
    bool CaptureInput::faceDetectEnabled(){
        return mFaceDetect != NULL;
    }
    
    vector<DetectRect> CaptureInput::getFaces(){
        if( faceDetectEnabled() ){
            return mFaceDetect->getFaces();
        }else{
            return vector<DetectRect>();
        }
    }
    
    // ========================================================================================
    //
    // Optical Flow.
    //
    // ========================================================================================
    
    void CaptureInput::setupOpticalFlow( Vec2i textureSize ){
        if( mOpticalFlow == NULL ){
            mOpticalFlow = new OpticalFlow( textureSize );
        }
    }
    
    void CaptureInput::updateOpticalFlow( float bias, float frameRate ){
        if( mOpticalFlow ){
            cout << "updateOpticalFlow()" << endl;
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
        return mOpticalFlow->getTexture();
    }
    
    Vec2i CaptureInput::getOpticalFlowTextureSize(){
        return mOpticalFlow->getSize();
    }
    
    Rectf CaptureInput::getOpticalFlowTextureBounds(){
        return mOpticalFlow->getBounds();
    }
    
}}