#include "CameraTexture.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    // public:
    
    CameraTexture::CameraTexture(){
        CameraTexture("*");
    };
    
    CameraTexture::CameraTexture( string deviceName ){
        CameraTexture( 640, 480, deviceName );
    };
    
    CameraTexture::CameraTexture( int32_t width, int32_t height, const string deviceName ){
        hrfm::display::Texture();
        hrfm::events::EventDispatcher();
        init( width, height, deviceName );
    };
    
    CameraTexture::~CameraTexture(){
        mCapture.reset();
    }
    
    void CameraTexture::init( int32_t width, int32_t height, const string deviceName ){
        
        cout << "CameraTexture" << endl;
        
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
            
            mTexture = gl::Texture( width, height );
            
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
    
    // protected:
    
    void CameraTexture::_update(){
        cout << mCapture << endl;
        if( mCapture && mCapture->checkNewFrame() ) {
            cout << "_update" << endl;
            mTexture = gl::Texture( mCapture->getSurface() );
        }
    }
    
}}