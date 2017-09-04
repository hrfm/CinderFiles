#include "App.hpp"

namespace hrfm{ namespace app{
    
    void App::setup(){
        
        try{
            
            // --- Load XML for Settings. --------------------------------------------
            
            string path = "setting.xml";
            
            for( int i=0; i<getCommandLineArgs().size(); i++ ){
                string key = getCommandLineArgs().at(i);
                if( key == "-xml" || key == "-x" ){
                    path = getCommandLineArgs().at(++i);
                    break;
                }
            }
            
            cout << "--- App::prepareSettings -----------------------------------------------" << endl;
            
            SiAppSettings & appSettings = SiAppSettings::getInstance();
            auto _settingXml = appSettings.init( this, path );
            
            cout << _settingXml << endl;
            
            // --- Log settings from XML. --------------------------------------------
            
            if( _settingXml.hasChild("setting/log") ){
                XmlTree log = _settingXml.getChild("setting/log");
                if( log.hasAttribute("dest") ){
                    ci::fs::path log_output = hrfm::fs::resolvePath( log.getAttributeValue<string>("dest") );
                    cout << endl << endl << "Log output > (" << log_output.native() << endl;
                    LOG_FILE = freopen( log_output.native().c_str(), "w", stdout );
                }
            }
            
            // --- Start preparing with XML. --------------------------------------------------
            
            setFrameRate(appSettings.getFramerate());
            cout << "FrameRate  : " << getFrameRate() << endl;
            
            setWindowSize( appSettings.getWindowSize() );
            cout << "WindowSize : " << getWindowSize() << endl << endl;
            
            // --- Setup from xml settings. -----------------------
            
            this->_setupFromXML(_settingXml.getChild("setting/setup") );
            
        }catch(...){
            
        }
        
        // --- TriggerEvent Listener.
        
        hrfm::events::SiEventDispatcher::getInstance().addEventListener( events::TriggerEvent::TRIGGER, this, &App::onTrigger );
        
        cout << "------------------------------------------------------------------------" << endl << endl;
        
    }
    
    void App::_setupFromXML(XmlTree &xml){
        if( xml.hasChild("window") ){
            auto windowXML = xml.getChild("window");
            _hideCursorOnFullscreen = ( windowXML.hasAttribute("hideCursor") && windowXML.getAttributeValue<string>("hideCursor") == "true" );
            if( windowXML.hasAttribute("fullScreen") && windowXML.getAttributeValue<string>("fullScreen") == "true" )
            {
                this->setFullScreen(true);
                ::system( ("open -a "+ci::app::App::getAppPath().string()).c_str() );
                if( this->isFullScreen() && _hideCursorOnFullscreen ){
                    this->hideCursor();
                }else{
                    this->showCursor();
                }
            }
        }
    }
    
    void App::update(){
        hrfm::utils::SiTimeUtil::getInstance().update();
        hrfm::app::SiAppInfo::getInstance().update();
    }
    
    void App::draw(){
        if( _isDebugMode ){
            _drawDebug();
        }
    }
    void App::_drawDebug(){
        drawFPS();
    }
    void App::drawFPS( vec2 position ){
        ci::gl::drawString( "FPS = " + toString(getAverageFps()) , position, Color::white() );
    }
    
    void App::keyDown( ci::app::KeyEvent event ){
        if( event.getChar() == 'f' ){
            this->setFullScreen( ! this->isFullScreen() );
            if( this->isFullScreen() && this->_hideCursorOnFullscreen ){
                this->hideCursor();
            }else{
                this->showCursor();
            }
        }
        if( event.isControlDown() && event.getChar() == 'd' ){
            _isDebugMode = !_isDebugMode;
        }
    }
    
    void App::onTrigger( events::TriggerEvent * event ){
        cout << "TRIGGER : " << event->type() << endl;
    }
    
}}

/*
namespace hrfm { namespace app{
    // === Update / Draw =========================================================
    void App::updateStage(){
        vector<DisplayNode*>::iterator it;
        vector<DisplayNode*>::iterator end = stage->children.end();
        for( it=stage->children.begin(); it!=end; ++it ){
            (*it)->size( getWindowSize() );
        }
        stage->update();
        stage->draw();
    }
    // === Event Listener. =======================================================
    void App::resize(){
        camera.setPerspective( 30, getWindowAspectRatio(), 0.1, 100 );
        stage->size( getWindowSize() );
    }
}}
*/
