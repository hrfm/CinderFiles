#include "SiCaptureInput.h"

namespace hrfm { namespace io{
    
    void SiCaptureInput::showAllDevices(){
        // print the devices
        cout << endl << "--- SiCaptureInput::showAllDevices() ---" << endl;
        for( auto device = ci::Capture::getDevices().begin(); device != ci::Capture::getDevices().end(); ++device ) {
            cout << "Device: " << (*device)->getName() << " " << std::endl;
        }
        cout << "----------------------------------------" << endl;
    }
    
    ci::CaptureRef SiCaptureInput::createRef( int width, int height, string deviceName ){
        
        if( _captureRefMap.find( deviceName ) != _captureRefMap.end() ){
            return getCaptureRef(deviceName);
        }
        
        ci::CaptureRef ref;
        
        try {
            if( deviceName == "*" ){
                ref = ci::Capture::create( width, height );
            }else{
                ref = ci::Capture::create( width, height, ci::Capture::findDeviceByName(deviceName) );
            }
            ref->start();
            _captureRefMap[deviceName] = ref;
        }catch( ... ) {
            cout << "Failed to initialize capture" << std::endl;
        }
        
        return ref;
        
    }
    
    ci::CaptureRef SiCaptureInput::getCaptureRef( string deviceName ){
        return _captureRefMap[deviceName];
    }

    ci::Surface SiCaptureInput::getSurface( string deviceName ){
        return _captureRefMap[deviceName]->getSurface();
    }
    
    ci::gl::Texture SiCaptureInput::getTexture( string deviceName ){
        return ci::gl::Texture( getSurface( deviceName ) );
    }
    
    void SiCaptureInput::update(){
        
    }
    
}}