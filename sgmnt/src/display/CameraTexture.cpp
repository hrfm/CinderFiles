#include "CameraTexture.h"

using namespace ci;

namespace sgmnt{ namespace display{
    
    CameraTexture::CameraTexture(){
        sgmnt::display::Texture();
        sgmnt::events::EventDispatcher();
    };
    
    CameraTexture::CameraTexture( string deviceName ){
        CameraTexture();
        init( 640, 480, deviceName );
    };
    
    CameraTexture::CameraTexture( int32_t width, int32_t height, const string deviceName ){
        CameraTexture();
        init( width, height, deviceName );
    };
    
    CameraTexture::~CameraTexture(){
        mCapture.reset();
    }
    
    void CameraTexture::init( int32_t width, int32_t height, const string deviceName ){
        
        // --- Setup Capture Devices. ---
        
        this->width  = width;
        this->height = height;
        
        showAllDevices();
        
        try {
            if( deviceName != "*" ){
                mCapture = Capture::create( width, height, Capture::findDeviceByName(deviceName) );
            }else{
                mCapture = Capture::create( width, height );
            }
            mCaptureAvailable = true;
        }catch( ... ) {
            cout << "Failed to initialize capture" << std::endl;
            mCaptureAvailable = false;
        }
        
    }
    
    void CameraTexture::showAllDevices(){
        // print the devices
        for( auto device = Capture::getDevices().begin(); device != Capture::getDevices().end(); ++device ) {
            cout << "Device: " << (*device)->getName() << " " << std::endl;
        }
    }
    
    bool CameraTexture::captureAvailable(){
        return mCaptureAvailable;
    }
    
    ci::CaptureRef CameraTexture::getCaptureRef(){
        return mCapture;
    }
    
    void CameraTexture::update(){
        if( mCapture && mCapture->checkNewFrame() ) {
            gl::Texture tex = gl::Texture( mCapture->getSurface() );
            mTexturePtr = &tex;
        }
    }
    
    void CameraTexture::_draw(){
        if( mCaptureAvailable ){
            gl::draw( *mTexturePtr, Rectf( 0, 0, width, height ) );
        }
    }
    
}}