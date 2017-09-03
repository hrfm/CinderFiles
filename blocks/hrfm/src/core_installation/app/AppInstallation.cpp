#include "AppInstallation.hpp"

#include "SiAudioInput.h"
#include "SiCaptureInput.h"
#include "SiKORGMIDIInterface.h"

namespace hrfm{ namespace app{
    
    void AppInstallation::setup(){
        hrfm::app::App::setup();
    }
    
    void AppInstallation::_setupFromXML( ci::XmlTree &xml){
        hrfm::app::App::_setupFromXML(xml);
        if( xml.hasChild("audio") ){
            this->_initAudio(xml.getChild("audio"));
        }else{
            useAudio = false;
        }
        if( xml.hasChild("capture") ){
            this->_initCapture(xml.getChild("capture"));
        }else{
            useCapture = false;
        }
        if( xml.hasChild("korg") ){
            this->_initKORGMIDIInterface(xml.getChild("korg"));
        }
    }
    
    void AppInstallation::_initAudio( ci::XmlTree &xml ){
        string deviceName = "*";
        if( xml.hasAttribute("deviceName") ){
            deviceName = xml.getAttributeValue<string>("deviceName");
        }
        if( xml.hasAttribute("bandSize") ){
            useAudio = true;
            int bandSize = xml.getAttributeValue<int>("bandSize");
            cout << "- Setup AudioInput with bandSize " << bandSize << endl;
            hrfm::io::SiAudioInput::getInstance().setup( deviceName, bandSize );
            cout << endl;
        }
    }
    
    void AppInstallation::_initCapture( ci::XmlTree &xml ){
        
        if( xml.hasAttribute("width") && xml.hasAttribute("height") ){
            
            useCapture        = true;
            ci::ivec2 captureSize = ci::ivec2( xml.getAttributeValue<int>("width"), xml.getAttributeValue<int>("height") );
            
            int cacheLength = 2;
            if( xml.hasAttribute("cacheLength") ){
                cacheLength = xml.getAttributeValue<int>("cacheLength");
            }
            
            if( xml.hasAttribute("deviceName") ){
                string deviceName = xml.getAttributeValue<string>("deviceName");
                cout << "- Setup CaptureInput [" + deviceName + "] with sized " << captureSize << endl;
                hrfm::io::SiCaptureInput::getInstance().setDefaultDeviceName(deviceName);
            }else{
                cout << "- Setup CaptureInput with sized " << captureSize << endl;
                hrfm::io::SiCaptureInput::getInstance().setDefaultDeviceName("*");
            }
            
            hrfm::io::SiCaptureInput::getInstance().createRef(captureSize.x,captureSize.y);
            
        }
        
    }
    
    void AppInstallation::_initKORGMIDIInterface( ci::XmlTree &xml ){
        if( xml.hasAttribute("listen") ){
            int listenPort = xml.getAttributeValue<int>("listen");
            cout << "- Setup KORGMIDIInterface with listen port " << listenPort << endl;
            hrfm::io::midi::SiKORGMIDIInterface::getInstance().listen( listenPort );
            cout << endl;
        }
    }
    
    void AppInstallation::update(){
        hrfm::app::App::update();
        updateIO();
    }
    
    void AppInstallation::updateIO(){
        // --- Update Inputs. ---
        if( useAudio ){
            hrfm::io::SiAudioInput::getInstance().update();
        }
        if( useCapture ){
            hrfm::io::SiCaptureInput::getInstance().update();
        }
        // --- Update Time. ---
        //hrfm::utils::SiTimeUtil::getInstance().update();
        //hrfm::app::SiAppInfo::getInstance().update( ci::app::App::getElapsedSeconds(), ci::app::App::getElapsedFrames() );
    }
    
    void AppInstallation::_drawDebug(){
        ci::gl::ScopedColor color( ColorA( 1.0, 1.0, 1.0, 1.0 ) );
        hrfm::app::App::_drawDebug();
        if( useAudio ){
            float w = getWindowWidth();
            float h = getWindowHeight();
            Rectf rect( w - 180, h - 70, w - 20, h - 20 );
            io::SiAudioInput::getInstance().drawWave( rect );
            io::SiAudioInput::getInstance().drawFFT( rect );
            io::SiAudioInput::getInstance().drawFFTRanged( rect );
        }
    }
    
}}