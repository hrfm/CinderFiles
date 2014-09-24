#pragma once

#include "cinder/app/AppNative.h"
#include "Cinder/Camera.h"
#include "cinder/Xml.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

// --- timeinfo ---

#include <time.h>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

// --- Inputs ---

#include "OscInput.h"
#include "AudioInput.h"
#include "CaptureInput.h"

#include "Utils.h"

using namespace std;

// ====================================

int * ___fader___ = new int[16];

void ___onNanokontrolMessage___( const sgmnt::Event &event){
    for( int i = 0; i < event.message.getNumArgs(); i++ ){
        ___fader___[i] = event.message.getArgAsInt32(i);
    }
}

// ====================================

Font ___font___ = Font("Helvetica",12);

namespace sgmnt{ namespace app{

    class AppBase : public ci::app::AppNative{
        
        public :
        
            AppBase(){
                ci::app::AppNative();
            }
            
            virtual void prepareSettings( Settings *settings ){
                
                cout << "--- AppBase::prepareSettings()" << endl;
                
                // --- Load XML for Settings.
                
                settingXml = XmlTree( loadResource( "../resources/setting.xml" ) );
                
                settingXml = loadXml( "setting_xml", "../resources/setting.xml" );
                
                cout << settingXml << endl;
                
                // --- Prepare settings from XML.
                
                XmlTree prepare = settingXml.getChild("setting/prepare");
                
                settings->setFrameRate( prepare.getAttributeValue<float>("frameRate") );
                
                mWindowSize = Vec2i( prepare.getAttributeValue<int>("windowWidth"), prepare.getAttributeValue<int>("windowHeight") );
                settings->setWindowSize( mWindowSize.x, mWindowSize.y );
                
                cout << endl;
                
            }
            
            virtual void resize(){
                cout << "- Setup Camera Perspective." << endl;
                camera.setPerspective( 30, getWindowAspectRatio(), 0.1, 100 );
            }
            
            virtual void setup(){
                
                // --- init ___fader___.
                
                for( int i=0; i< 16;i++ ){
                    ___fader___[i] = 0;
                }
                
                // ---
                
                cout << "--- AppBase::setup()" << endl << endl;
                
                // --- Setup from XML.
                
                XmlTree setupSettings = settingXml.getChild("setting/setup");
                
                if( setupSettings.hasChild("window") ){
                    
                    XmlTree window = setupSettings.getChild("window");
                    
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
                
                if( setupSettings.hasChild("osc") ){
                    
                    if( setupSettings.getChild("osc").hasAttribute("port") ){
                        
                        useOsc = true;
                        
                        int port = setupSettings.getChild("osc").getAttributeValue<int>("port");
                        cout << "- Setup OSCInput on port " << port << endl << endl;
                        
                        oscInput.setup( port );
                        setupOsc();
                        
                    }
                    
                }
                
                if( setupSettings.hasChild("audio") ){
                    
                    if( setupSettings.getChild("audio").hasAttribute("bandSize") ){
                        
                        useAudio = true;
                        
                        int bandSize = setupSettings.getChild("audio").getAttributeValue<int>("bandSize");
                        cout << "- Setup AudioInput with bandSize " << bandSize << endl;
                        audioInput.setup( bandSize );
                        if( useOsc ){
                            audioInput.useAudioManager( oscInput );
                        }
                        
                        cout << endl;
                        
                    }
                    
                }
                
                if( setupSettings.hasChild("capture") ){
                    
                    XmlTree capture = setupSettings.getChild("capture");
                    
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
                
                resize();
                
                // --- init timeinfo.
                
                time_t rawtime = time(NULL);
                time(&rawtime);
                timeinfo = localtime(&rawtime);
                
                cout << endl;
                
            }
        
            /**
             * node_name で指定したノード名で設定されている XML を読み込みます.
             */
            XmlTree loadXml( string node_name ){
                XmlTree xmlList  = settingXml.getChild("setting/xml_list");
                string file_name = xmlList.getChild(node_name).getAttributeValue<string>("file_name");
                return loadXml( node_name, "../resources/" + file_name );
            }
            
            XmlTree loadXml( string node_name, string local_path ){
                
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

        
            string getTimeString(const char *format){
                char str[256];
                strftime( str, 255, format, timeinfo );
                string s = str;
                return s;
            }
        
            virtual void setColor( float red, float green, float blue ){
                mColor.set( red, green, blue );
            }
            virtual Color getColor(){
                return mColor;
            }
            virtual ColorA getColorA( float alpha ){
                return ColorA( mColor.r, mColor.g, mColor.b, alpha );
            }
            
            virtual Vec2i getWindowSize(){
                return mWindowSize;
            }
            
            virtual Vec2i getWindowAspect(){
                return sgmnt::utils::getAspectRatio( mWindowSize );
            }
        
            virtual void update(){
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
                time_t rawtime = time(NULL);
                time(&rawtime);
                timeinfo = localtime(&rawtime);
            }
            
            virtual void draw(){
                gl::clear();
                if( backgroundTex ){
                    gl::draw( backgroundTex, ci::app::getWindowBounds() );
                }
            }
            
            virtual void keyDown( ci::app::KeyEvent event ){
                if( event.getChar() == 'f' )
                    ci::app::setFullScreen( ! ci::app::isFullScreen() );
            }
        
            virtual float getFaderValueAt( int index ){
                return (float)___fader___[index] / 128;
            }
        
            virtual void setTextureToMap( string key, ci::gl::Texture tex ){
                mTextureMap[key] = tex;
            }
        
            virtual void setTextureToMap( string key, string path ){
                try {
                    cout << "- Loading Texture : " << path << endl;
                    setTextureToMap(key, gl::Texture( loadImage( loadResource( path ) ) ));
                }catch( ... ) {
                    console() << "unable to load the texture file!" << std::endl;
                }
            }
        
            virtual ci::gl::Texture getTextureFromMap( string key ){
                return mTextureMap[key];
            }
            
            virtual void drawFPS(){
                gl::drawString(
                    "FPS = " + toString(getAverageFps()) ,
                    Vec2f(20.0f,20.0f),
                    Color::white(),
                    ___font___
                );
            }
        
            // --- Common.
            
            XmlTree                 settingXml;
            gl::Texture             backgroundTex;
            CameraPersp             camera;
        
            struct tm *             timeinfo;
            
            // --- Osc
            bool                    useOsc;
            sgmnt::io::OscInput     oscInput;
            
            // --- Audio
            bool                    useAudio;
            sgmnt::io::AudioInput   audioInput;
        
            // --- Capture
            bool                    useCapture;
            bool                    doUpdateCapture;
            sgmnt::io::CaptureInput captureInput;
            
        protected :
        
            string  mRemoteUrl;
        
            Vec2i   mWindowSize;
            Color   mColor;
            
            map<string,ci::gl::Texture> mTextureMap;
        
            virtual void setupOsc(){
                // ===========================================
                // == Write osc code if needed.
                oscInput.addEventListener( "/nanokontrol", &___onNanokontrolMessage___ );
                // ===========================================
            }
        
    };
    
}}