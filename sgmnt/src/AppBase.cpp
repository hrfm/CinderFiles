#include "AppBase.h"

// === NAMESPACE ===================================================================================================

using namespace std;
using namespace sgmnt;
using namespace sgmnt::app;
using namespace sgmnt::io;

// === GLOBAL PROPERTY =============================================================================================

Font ___font___ = Font("Helvetica",12);

namespace sgmnt { namespace app{
    
    // ===========================================================================
    // === For Setup =============================================================
    
    void AppBase::prepareSettings( Settings *settings ){
        
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
        this->updateTimeinfo();
        
        resize();
        
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
                ci::app::setFullScreen(true);
                system( ("open -a "+ci::app::getAppPath().string()).c_str() );
            }
            if( window.hasAttribute("hideCursor") && window.getAttributeValue<string>("hideCursor") == "true" ){
                ci::app::AppNative::hideCursor();
            }
            if( window.hasAttribute("background") ){
                string path = window.getAttributeValue<string>("background");
                cout << "- Loading background image : " << path << endl;
                try {
                    backgroundTex = gl::Texture( loadImage( loadResource( path ) ) );
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
                oscInput.setup( port );
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
                audioInput.setup( bandSize );
                if( useOsc ){
                    audioInput.useAudioManager( oscInput );
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
        // --- Update Inputs. ---
        if( useOsc ){
            oscInput.update();
        }
        if( useAudio ){
            audioInput.update();
        }
        if( useCapture && doUpdateCapture ){
            captureInput.update();
        }
        // --- Update Time. ---
        this->updateTimeinfo();
    }
    
    void AppBase::draw(){
        gl::clear();
        if( backgroundTex ){
            gl::draw( backgroundTex, ci::app::getWindowBounds() );
        }
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
        return sgmnt::utils::getAspectRatio( mWindowSize );
    }
    
    float AppBase::getFaderValueAt( int index ){
        return (float)nanoKontrolFader[index] / 128;
    }
    
    void AppBase::setTextureToMap( string key, ci::gl::Texture tex ){
        mTextureMap[key] = tex;
    }
    
    void AppBase::setTextureToMap( string key, string path ){
        try {
            cout << "- Loading Texture : " << path << endl;
            setTextureToMap(key, gl::Texture( loadImage( loadResource( path ) ) ));
        }catch( ... ) {
            console() << "unable to load the texture file!" << std::endl;
        }
    }
    
    ci::gl::Texture AppBase::getTextureFromMap( string key ){
        return mTextureMap[key];
    }
    
    // ===========================================================================
    // === Utility ===============================================================
    
    void AppBase::updateTimeinfo(){
        time_t rawtime = time(NULL);
        time(&rawtime);
        timeinfo = localtime(&rawtime);
    }
    
    string AppBase::getTimeString(const char *format){
        char str[256];
        strftime( str, 255, format, timeinfo );
        string s = str;
        return s;
    }
    
    /**
     * node_name で指定したノード名で設定されている XML を読み込みます.
     */
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
    }
    
    void AppBase::keyDown( ci::app::KeyEvent event ){
        if( event.getChar() == 'f' )
            ci::app::setFullScreen( ! ci::app::isFullScreen() );
    }
    
    void AppBase::onReceiveOscMessage( OscInputEvent & event ){
        for( int i = 0; i < event.message.getNumArgs(); i++ ){
            nanoKontrolFader[i] = event.message.getArgAsInt32(i);
        }
    }
    
    // ===========================================================================
    // === For Debug. ============================================================
    
    void AppBase::drawFPS( Vec2f position ){
        gl::drawString(
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
        oscInput.addEventListener("/nanokontrol",this,&AppBase::onReceiveOscMessage);
        // ===========================================
    }
    
}}
