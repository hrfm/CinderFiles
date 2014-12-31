#include "AppBase.h"
#include <iostream>

// === NAMESPACE ===================================================================================================

using namespace std;
using namespace hrfm;
using namespace hrfm::app;
using namespace hrfm::events;
using namespace hrfm::io;

// === GLOBAL PROPERTY =============================================================================================

namespace hrfm { namespace app{
    
    void AppBase::prepareSettings( Settings *settings ){
        
        // --- Load XML for Settings. --------------------------------------------
        
        string path = "setting.xml";
        
        for( int i=0; i<getArgs().size(); i++ ){
            string key = getArgs().at(i);
            if( key == "-xml" || key == "-x" ){
                path = getArgs().at(++i);
                break;
            }
        }
        
        XmlTree _settingXml = SiSetting::getInstance().load(path);
        
        // --- Log settings from XML. --------------------------------------------
        
        if( _settingXml.hasChild("setting/log") ){
            XmlTree log = _settingXml.getChild("setting/log");
            if( log.hasAttribute("dest") ){
                fs::path log_output = generateFilePath( log.getAttributeValue<string>("dest") );
                LOG_FILE = freopen( log_output.native().c_str(), "w", stdout );
            }
        }
        
        // --- Start preparing. --------------------------------------------------
        
        cout << "--- AppBase::prepareSettings() -----------------------------------------" << endl << endl;
        cout << "Setting xml from \"" << path << "\"" <<  endl << endl;
        cout << _settingXml;
        
        if( _settingXml.hasChild("setting/prepare") ){
            
            XmlTree prepare = _settingXml.getChild("setting/prepare");
            
            // Set this app's FrameRate from xml.
            if( prepare.hasAttribute("frameRate") ){
                settings->setFrameRate( prepare.getAttributeValue<float>("frameRate") );
            }
            cout << "FrameRate  : " << settings->getFrameRate() << endl;
            
            // Set this app's WindowSize from xml.
            if( prepare.hasAttribute("windowWidth") && prepare.hasAttribute("windowHeight") ){
                mWindowSize = Vec2i( prepare.getAttributeValue<int>("windowWidth"), prepare.getAttributeValue<int>("windowHeight") );
            }else{
                mWindowSize = settings->getWindowSize();
            }
            settings->setWindowSize( mWindowSize.x, mWindowSize.y );
            cout << "WindowSize : " << settings->getWindowSize() << endl << endl;
            
        }
        
        cout << "------------------------------------------------------------------------" << endl << endl;
        
    }
    
    void AppBase::setup(){
        
        cout << "--- AppBase::setup() ---------------------------------------------------" << endl << endl;
        
        // --- Setup from xml settings. -----------------------
        
        XmlTree xml = SiSetting::getInstance().getXmlTree().getChild("setting/setup");
        {
            if( xml.hasChild("window") ){
                this->initWindow( xml.getChild("window") );
            }
            if( xml.hasChild("audio") ){
                this->initAudio(xml.getChild("audio"));
            }
            if( xml.hasChild("capture") ){
                this->initCapture(xml.getChild("capture"));
            }
            if( xml.hasChild("korg") ){
                this->initKORGMIDIInterface(xml.getChild("korg"));
            }
        }
        
        // --- TriggerEvent Listener.
        
        SiEventDispatcher::getInstance().addEventListener( TriggerEvent::TRIGGER, this, &AppBase::onTrigger );
        
        // ---
        
        resize();
        
        cout << "------------------------------------------------------------------------" << endl << endl;
        
    }
    
    void AppBase::initWindow(XmlTree &xml){
        if( xml.hasAttribute("fullScreen") && xml.getAttributeValue<string>("fullScreen") == "true" ){
            ci::app::setFullScreen(true);
            system( ("open -a "+ci::app::getAppPath().string()).c_str() );
        }
        if( xml.hasAttribute("hideCursor") && xml.getAttributeValue<string>("hideCursor") == "true" ){
            ci::app::AppNative::hideCursor();
        }
    }
    
    void AppBase::initAudio( XmlTree &xml ){
        string deviceName = "*";
        if( xml.hasAttribute("deviceName") ){
            deviceName = xml.getAttributeValue<string>("deviceName");
        }
        if( xml.hasAttribute("bandSize") ){
            useAudio = true;
            int bandSize = xml.getAttributeValue<int>("bandSize");
            cout << "- Setup AudioInput with bandSize " << bandSize << endl;
            SiAudioInput::getInstance().setup( deviceName, bandSize );
            cout << endl;
        }
    }
    
    void AppBase::initCapture( XmlTree &xml ){
        
        if( xml.hasAttribute("width") && xml.hasAttribute("height") ){
            
            useCapture      = true;
            Vec2i captureSize = Vec2i( xml.getAttributeValue<int>("width"), xml.getAttributeValue<int>("height") );
            
            int cacheLength = 2;
            if( xml.hasAttribute("cacheLength") ){
                cacheLength = xml.getAttributeValue<int>("cacheLength");
            }
            
            if( xml.hasAttribute("deviceName") ){
                string deviceName = xml.getAttributeValue<string>("deviceName");
                cout << "- Setup CaptureInput [" + deviceName + "] with sized " << captureSize << endl;
                captureInput.setup( captureSize.x, captureSize.y, deviceName, cacheLength );
            }else{
                cout << "- Setup CaptureInput with sized " << captureSize << endl;
                captureInput.setup( captureSize.x, captureSize.y, "*", cacheLength );
            }
            
        }
        
    }
    
    void AppBase::initKORGMIDIInterface( XmlTree &xml ){
        if( xml.hasAttribute("listen") ){
            int listenPort = xml.getAttributeValue<int>("listen");
            cout << "- Setup KORGMIDIInterface with listen port " << listenPort << endl;
            SiKORGMIDIInterface::getInstance().listen( listenPort );
            cout << endl;
        }
    }
    
    // === Setter / Getter =======================================================
    
    Vec2i AppBase::getWindowSize(){
        return mWindowSize;
    }
    
    Vec2i AppBase::getWindowAspect(){
        return hrfm::utils::getAspectRatio( mWindowSize );
    }
    
    // === Update / Draw =========================================================
    
    void AppBase::update(){
        updateIO();
        updateStage();
    }
    
    void AppBase::updateIO(){
        
        // --- Update Inputs. ---
        
        if( SiOscInput::getInstance().listening() ){
            SiOscInput::getInstance().update();
        }
        
        if( useAudio ){
            SiAudioInput::getInstance().update();
        }
        
        if( useCapture ){
            SiCaptureInput::getInstance().update();
            captureInput.update();
        }
        
        // --- Update Time. ---
        hrfm::utils::SiTimeUtil::getInstance().update();
        
    }
    
    void AppBase::updateStage(){
        stage.update();
    }
    
    void AppBase::draw(){
        ci::gl::clear();
        stage.draw();
        if( _isDrawFPS ){
            drawFPS();
        }
    }
    
    void AppBase::drawFPS( Vec2f position ){
        ci::gl::drawString( "FPS = " + toString(getAverageFps()) , position, Color::white(), ___font___ );
    }
    
    // === Event Listener. =======================================================
    
    void AppBase::resize(){
        camera.setPerspective( 30, getWindowAspectRatio(), 0.1, 100 );
        stage.setSize( getWindowWidth(), getWindowHeight() );
    }
    
    void AppBase::keyDown( ci::app::KeyEvent event ){
        if( event.getChar() == 'f' ){
            ci::app::setFullScreen( ! ci::app::isFullScreen() );
        }
        if( event.isControlDown() && event.getChar() == 'd' ){
            _isDrawFPS = !_isDrawFPS;
        }
    }
    
    void AppBase::onTrigger( TriggerEvent * event ){
        cout << "TRIGGER : " << event->type() << endl;
    }
    
}}
