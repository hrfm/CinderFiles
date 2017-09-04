#include "SiAppSettings.hpp"

namespace hrfm { namespace app{
    
    ci::XmlTree SiAppSettings::init( hrfm::app::App * app, ci::fs::path settingXmlPath ){
        _app = app;
        _xml = hrfm::io::XmlLoader::load( settingXmlPath );
        return _xml;
    }
    
    hrfm::app::App * SiAppSettings::getApp(){
        return _app;
    }
    
    ci::XmlTree SiAppSettings::getXml(){
        return _xml;
    }
    
    int SiAppSettings::getFramerate(){
        if( !hasValue("frameRate") ){
            setValue("frameRate", 30.0f);
            if( _xml.hasChild("setting/prepare") ){
                XmlTree prepare = _xml.getChild("setting/prepare");
                // Set this app's FrameRate from xml.
                if( prepare.hasAttribute("frameRate") ){
                    setValue( "frameRate", prepare.getAttributeValue<float>("frameRate") );
                }
            }
        }
        return getValue<float>("frameRate");
    }
    
    ivec2 SiAppSettings::getWindowSize(){
        if( !hasValue("windowSize") ){
            if( _xml.hasChild("setting/prepare") ){
                XmlTree prepare = _xml.getChild("setting/prepare");
                // Set this app's WindowSize from xml.
                if( prepare.hasAttribute("windowWidth") && prepare.hasAttribute("windowHeight") ){
                    setValue("windowSize", ci::ivec2( prepare.getAttributeValue<int>("windowWidth"), prepare.getAttributeValue<int>("windowHeight") ) );
                }else{
                    setValue("windowSize", ci::ivec2( _app->getWindowSize() ) );
                }
            }else{
                setValue("windowSize", ci::ivec2( _app->getWindowSize() ) );
            }
        }
        return getValue<ci::ivec2>("windowSize");
    }
    
}}