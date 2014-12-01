#include "AppBase.h"
#include <iostream>

// === NAMESPACE ===================================================================================================

using namespace std;
using namespace hrfm;
using namespace hrfm::app;
using namespace hrfm::events;
using namespace hrfm::io;

// === GLOBAL PROPERTY =============================================================================================

Font ___font___ = Font("Helvetica",12);

FILE * LOG_FILE;

namespace hrfm { namespace app{
    
    // ===========================================================================
    // === For Setup =============================================================
    
    void AppBase::prepareSettings( Settings *settings ){
        
        fs::path log_output = getDocumentsDirectory() / "log.txt";
        //LOG_FILE = freopen( log_output.native().c_str(), "w", stdout );
        
        cout << "--- AppBase::prepareSettings()" << endl;
        
        // --- Load XML for Settings.
        
        settingXml = XmlTree( loadResource( "../resources/setting.xml" ) );
        settingXml = loadXml( "setting_xml", "../resources/setting.xml" );
        
        cout << settingXml << endl;
        cout << endl;
        
        // --- Prepare settings from XML.
        XmlTree prepare = settingXml.getChild("setting/prepare");
        
        // Set this app's FrameRate from xml.
        settings->setFrameRate( prepare.getAttributeValue<float>("frameRate") );
        settings->setFrameRate( 60 );
        cout << "FrameRate  : " << settings->getFrameRate() << endl;
        
        // Set this app's WindowSize from xml.
        mWindowSize = Vec2i( prepare.getAttributeValue<int>("windowWidth"), prepare.getAttributeValue<int>("windowHeight") );
        settings->setWindowSize( mWindowSize.x, mWindowSize.y );
        cout << "WindowSize : " << settings->getWindowSize() << endl;
        
        cout << endl;
        
    }
    
    void AppBase::setup(){
        
        cout << "--- AppBase::setup()" << endl << endl;
        
        XmlTree setupSettings = settingXml.getChild("setting/setup");
        
        this->initFader();
        this->initWindow(setupSettings);
        this->initOSC(setupSettings);
        this->initAudio(setupSettings);
        this->initCapture(setupSettings);
        
        resize();
        
        stage.setSize( getWindowWidth(), getWindowHeight() );
        
        // --- TriggerEvent Listener.
        
        SiEventDispatcher::getInstance().addEventListener( TriggerEvent::TRIGGER, this, &AppBase::onTrigger );
        
        cout << endl;
        
    }
    
    void AppBase::initFader(){
        for( int i=0; i< 16;i++ ){
            nanoKontrolFader[i] = 0;
        }
    }
    
    void AppBase::initWindow(XmlTree &xml){
        if( xml.hasChild("window") ){
            XmlTree window = xml.getChild("window");
            if( window.hasAttribute("fullScreen") && window.getAttributeValue<string>("fullScreen") == "true" ){
                //ci::app::setFullScreen(true);
                system( ("open -a "+ci::app::getAppPath().string()).c_str() );
            }
            if( window.hasAttribute("hideCursor") && window.getAttributeValue<string>("hideCursor") == "true" ){
                ci::app::AppNative::hideCursor();
            }
            if( window.hasAttribute("background") ){
                string path = window.getAttributeValue<string>("background");
                cout << "- Loading background image : " << path << endl;
                try {
                    background.init( loadImage( loadResource( path ) ) );
                    stage.addChild(&background);
                }catch( ... ) {
                    cout << "Unable to load Background texture file." << endl;
                }
                cout << endl;
            }
        }
    }
    
    void AppBase::initOSC( XmlTree &xml ){
        if( xml.hasChild("osc") ){
            XmlTree osc = xml.getChild("osc");
            if( osc.hasAttribute("port") ){
                useOsc = true;
                int port = osc.getAttributeValue<int>("port");
                cout << "- Setup OSCInput on port " << port << endl << endl;
                SiOscInput::getInstance().addListenPort( port );
                setupOsc();
            }
        }
    }
    
    void AppBase::initAudio( XmlTree &xml ){
        if( xml.hasChild("audio") ){
            XmlTree audio = xml.getChild("audio");
            if( audio.hasAttribute("bandSize") ){
                useAudio = true;
                int bandSize = audio.getAttributeValue<int>("bandSize");
                cout << "- Setup AudioInput with bandSize " << bandSize << endl;
                SiAudioInput::getInstance().setup( bandSize );
                if( useOsc ){
                    SiAudioInput::getInstance().useAudioManager();
                }
                cout << endl;
            }
        }
    }
    
    void AppBase::initCapture( XmlTree &xml ){
        if( xml.hasChild("capture") ){
            XmlTree capture = xml.getChild("capture");
            if( capture.hasAttribute("width") && capture.hasAttribute("height") ){
                useCapture      = true;
                doUpdateCapture = true;
                Vec2i captureSize = Vec2i( capture.getAttributeValue<int>("width"), capture.getAttributeValue<int>("height") );
                if( capture.hasAttribute("deviceName") ){
                    string deviceName = capture.getAttributeValue<string>("deviceName");
                    cout << "- Setup CaptureInput [ " + deviceName + " ]with sized " << captureSize << endl;
                    captureInput.setup( captureSize.x, captureSize.y, deviceName );
                }else{
                    cout << "- Setup CaptureInput with sized " << captureSize << endl;
                    captureInput.setup( captureSize.x, captureSize.y );
                }
                cout << endl;
            }
        }
    }
    
    // ===========================================================================
    // === Update / Draw =========================================================
    
    void AppBase::update(){
        updateIO();
        updateStage();
    }
    
    void AppBase::updateIO(){
        
        // --- Update Inputs. ---
        
        if( useOsc ){
            SiOscInput::getInstance().update();
        }
        
        if( useAudio ){
            SiAudioInput::getInstance().update();
        }
        
        if( useCapture && doUpdateCapture ){
            captureInput.update();
        }
        
    }
    
    void AppBase::updateStage(){
        
        // --- Update Stage. ---
        background.setSize( ci::app::getWindowSize() );
        stage.update();
        
        // --- Update Time. ---
        hrfm::utils::SiTimeUtil::getInstance().update();
        
    }
    
    void AppBase::draw(){
        ci::gl::clear();
        stage.draw();
    }
    
    // ===========================================================================
    // === Setter Getter =========================================================
    
    void AppBase::setColor( float red, float green, float blue ){
        mColor.set( red, green, blue );
    }
    
    Color AppBase::getColor(){
        return mColor;
    }
    
    ColorA AppBase::getColorA( float alpha ){
        return ColorA( mColor.r, mColor.g, mColor.b, alpha );
    }
    
    Vec2i AppBase::getWindowSize(){
        return mWindowSize;
    }
    
    Vec2i AppBase::getWindowAspect(){
        return hrfm::utils::getAspectRatio( mWindowSize );
    }
    
    float AppBase::getFaderValueAt( int index ){
        return (float)nanoKontrolFader[index] / 128;
    }
    
    // ===========================================================================
    // === Utility ===============================================================
    
    XmlTree AppBase::loadXml( string node_name ){
        XmlTree xmlList  = settingXml.getChild("setting/xml_list");
        string file_name = xmlList.getChild(node_name).getAttributeValue<string>("file_name");
        return loadXml( node_name, "../resources/" + file_name );
    }
    
    XmlTree AppBase::loadXml( string node_name, string local_path ){
        
        cout << endl << "--- AppBase::loadXml(" << node_name << ", " << local_path << ")" << endl;
        
        XmlTree xml;
        
        XmlTree xmlList  = settingXml.getChild("setting/xml_list");
        string url       = xmlList.getAttributeValue<string>("url");
        string file_name = xmlList.getChild(node_name).getAttributeValue<string>("file_name");
        
        try{
            cout << "Loading " << file_name << " from " + url << endl;
            xml = XmlTree( loadUrl( Url( url + file_name ) ) );
        }catch(...){
            cout << "Load error from remote. Try to load from local." << endl;
            xml = XmlTree( loadResource( local_path ) );
        }
        
        cout << endl;
        
        return xml;
        
    }
    
    // ===========================================================================
    // === Event Listener. =======================================================
    
    void AppBase::resize(){
        cout << "- Setup Camera Perspective." << endl;
        camera.setPerspective( 30, getWindowAspectRatio(), 0.1, 100 );
        stage.setSize( getWindowWidth(), getWindowHeight() );
    }
    
    void AppBase::keyDown( ci::app::KeyEvent event ){
        if( event.getChar() == 'f' )
            ci::app::setFullScreen( ! ci::app::isFullScreen() );
    }
    
    void AppBase::onReceiveOscMessage( OscInputEvent * event ){
        for( int i = 0; i < event->message.getNumArgs(); i++ ){
            nanoKontrolFader[i] = event->message.getArgAsInt32(i);
        }
    }
    
    void AppBase::onTrigger( TriggerEvent * event ){
        cout << "TRIGGER : " << event->type() << endl;
    }
    
    // ===========================================================================
    // === For Debug. ============================================================
    
    void AppBase::drawFPS( Vec2f position ){
        ci::gl::drawString(
                       "FPS = " + toString(getAverageFps()) ,
                       position,
                       Color::white(),
                       ___font___
                       );
    }
    
    // ===========================================================================
    // === OSC. ===============================================================
    
    void AppBase::setupOsc(){
        // ===========================================
        // == Write osc code if needed.
        SiOscInput::getInstance().addEventListener("/nanokontrol",this,&AppBase::onReceiveOscMessage);
        // ===========================================
    }
    
}}
