#include "CaptureInput.h"

using namespace ci;
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
        if( mCapture->getSurface() ){
            if( mFaceDetect ){
                mFaceDetect->update( mCapture->getSurface()->clone() );
            }
        }
    }
    
    ivec2 CaptureInput::getSize(){
        return mCapture->getSize();
    }
    
    Rectf CaptureInput::getBounds(){
        return mCapture->getBounds();
    }
    
    ci::gl::TextureRef CaptureInput::getCaptureTexture(){
        return SiCaptureInput::getInstance().getTexture( _deviceName );
    }
    
    ci::gl::TextureRef CaptureInput::getDiffTexture(){
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
    
    void CaptureInput::setupFaceDetect( ivec2 textureSize ){
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
    
}}