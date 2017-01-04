#include "AppBase.h"
#include <iostream>
#include "cinder/Utilities.h"

// === NAMESPACE ===================================================================================================

using namespace std;
using namespace hrfm;
using namespace hrfm::app;
using namespace hrfm::events;
using namespace hrfm::io;

// === GLOBAL PROPERTY =============================================================================================

namespace hrfm { namespace app{
    
    void AppBase::setup(){
        
        // --- Load XML for Settings. --------------------------------------------
        
        string path = "setting.xml";
        
        for( int i=0; i<getCommandLineArgs().size(); i++ ){
            string key = getCommandLineArgs().at(i);
            if( key == "-xml" || key == "-x" ){
                path = getCommandLineArgs().at(++i);
                break;
            }
        }
        
        XmlTree _settingXml = SiSetting::getInstance().load(path);
        
        // --- Log settings from XML. --------------------------------------------
        
        if( _settingXml.hasChild("setting/log") ){
            XmlTree log = _settingXml.getChild("setting/log");
            if( log.hasAttribute("dest") ){
                ci::fs::path log_output = hrfm::fs::resolvePath( log.getAttributeValue<string>("dest") );
                LOG_FILE = freopen( log_output.native().c_str(), "w", stdout );
            }
        }
        
        // --- Start preparing. --------------------------------------------------
        
        cout << "--- AppBase::prepareSettings(ver 0.8.x) -----------------------------------------" << endl << endl;
        cout << "Setting xml from \"" << path << "\"" <<  endl << endl;
        cout << _settingXml;
        
        if( _settingXml.hasChild("setting/prepare") ){
            
            XmlTree prepare = _settingXml.getChild("setting/prepare");
            
            // Set this app's FrameRate from xml.
            if( prepare.hasAttribute("frameRate") ){
                setFrameRate( prepare.getAttributeValue<float>("frameRate") );
            }
            cout << "FrameRate  : " << getFrameRate() << endl;
            
            // Set this app's WindowSize from xml.
            if( prepare.hasAttribute("windowWidth") && prepare.hasAttribute("windowHeight") ){
                mWindowSize = ivec2( prepare.getAttributeValue<int>("windowWidth"), prepare.getAttributeValue<int>("windowHeight") );
            }else{
                mWindowSize = getWindowSize();
            }
            setWindowSize( mWindowSize.x, mWindowSize.y );
            cout << "WindowSize : " << getWindowSize() << endl << endl;
            
        }
        
        cout << "--- AppBase::setup() ---------------------------------------------------" << endl << endl;
        
        // --- Setup from xml settings. -----------------------
        
        XmlTree xml = SiSetting::getInstance().getXmlTree().getChild("setting/setup");
        {
            if( xml.hasChild("window") ){
                this->initWindow( xml.getChild("window") );
            }
            if( xml.hasChild("audio") ){
                this->initAudio(xml.getChild("audio"));
            }else{
                useAudio = false;
            }
            if( xml.hasChild("capture") ){
                this->initCapture(xml.getChild("capture"));
            }else{
                useCapture = false;
            }
            if( xml.hasChild("korg") ){
                this->initKORGMIDIInterface(xml.getChild("korg"));
            }
        }
        
        // --- TriggerEvent Listener.
        
        SiEventDispatcher::getInstance().addEventListener( TriggerEvent::TRIGGER, this, &AppBase::onTrigger );
        
        // ---
        
        stage = hrfm::display::Stage::create();
        
        // ---
        
        resize();
        
        cout << "------------------------------------------------------------------------" << endl << endl;
        
    }
    
    void AppBase::initWindow(XmlTree &xml){
        
        _hideCursor = ( xml.hasAttribute("hideCursor") && xml.getAttributeValue<string>("hideCursor") == "true" );
        
        if( xml.hasAttribute("fullScreen") && xml.getAttributeValue<string>("fullScreen") == "true" )
        {
            ci::app::setFullScreen(true);
            ::system( ("open -a "+ci::app::getAppPath().string()).c_str() );
            if( ci::app::isFullScreen() && _hideCursor ){
                ci::app::App::hideCursor();
            }else{
                ci::app::App::showCursor();
            }
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
            
            useCapture        = true;
            ivec2 captureSize = ivec2( xml.getAttributeValue<int>("width"), xml.getAttributeValue<int>("height") );
            
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
    
    ivec2 AppBase::getWindowSize(){
        return mWindowSize;
    }
    
    // === Update / Draw =========================================================
    
    void AppBase::update(){
        updateIO();
        updateStage();
    }
    
    void AppBase::updateIO(){
        
        // --- Update Inputs. ---
        
        if( useAudio ){
            SiAudioInput::getInstance().update();
        }
        
        if( useCapture ){
            SiCaptureInput::getInstance().update();
            captureInput.update();
        }
        
        // --- Update Time. ---
        
        hrfm::utils::SiTimeUtil::getInstance().update();
        hrfm::app::SiAppInfo::getInstance().update();
        
    }
    
    void AppBase::updateStage(){
        vector<DisplayNode*>::iterator it;
        vector<DisplayNode*>::iterator end = stage->children.end();
        for( it=stage->children.begin(); it!=end; ++it ){
            (*it)->size = getWindowSize();
        }
        stage->update();
        stage->draw();
    }
    
    void AppBase::draw(){
        ci::gl::clear();
        ci::gl::draw( stage->getTexture(), getWindowBounds() );
        if( _isDebugMode ){
            //cout << "debug" << endl;
            _drawDebug();
        }
    }

    void AppBase::drawFPS( vec2 position ){
        ci::gl::drawString( "FPS = " + toString(getAverageFps()) , position, Color::white(), ___font___ );
    }
    
    void AppBase::coutGLInfo(){
        cout << "Vendor : " << glGetString(GL_VENDOR)     << '\n';
        cout << "GPU : " << glGetString(GL_RENDERER)   << '\n';
        cout << "OpenGL ver. " << glGetString(GL_VERSION) << '\n';
        cout << "Extensions : " << endl;
        cout << glGetString(GL_EXTENSIONS) << endl;
    }
    
    // === Event Listener. =======================================================
    
    void AppBase::resize(){
        camera.setPerspective( 30, getWindowAspectRatio(), 0.1, 100 );
        stage->size = getWindowSize();
    }
    
    void AppBase::keyDown( ci::app::KeyEvent event ){
        if( event.getChar() == 'f' ){
            ci::app::setFullScreen( ! ci::app::isFullScreen() );
            if( ci::app::isFullScreen() && _hideCursor ){
                ci::app::App::hideCursor();
            }else{
                ci::app::App::showCursor();
            }
        }
        if( event.isControlDown() && event.getChar() == 'd' ){
            _isDebugMode = !_isDebugMode;
        }
    }
    
    void AppBase::onTrigger( TriggerEvent * event ){
        cout << "TRIGGER : " << event->type() << endl;
    }
    
    //!private
    
    void AppBase::_drawDebug(){
        ci::gl::color( ColorA( 1.0, 1.0, 1.0, 1.0 ) );
        drawFPS();
        if( useAudio ){
            float w = getWindowWidth();
            float h = getWindowHeight();
            Rectf rect( w - 180, h - 70, w - 20, h - 20 );
            SiAudioInput::getInstance().drawWave( rect );
            SiAudioInput::getInstance().drawFFT( rect );
            SiAudioInput::getInstance().drawFFTRanged( rect );
        }
    }
    
}}
