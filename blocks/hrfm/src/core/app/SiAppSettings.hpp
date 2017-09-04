#pragma once

#include "cinder/Xml.h"
#include "DynamicMap.h"
#include "Singleton.h"
#include "XmlLoader.h"
#include "App.hpp"

using namespace ci;

namespace hrfm { namespace app{
    
    class App;
    
    class SiAppSettings : public hrfm::utils::Singleton<SiAppSettings>, public hrfm::utils::DynamicMap{
        
    public:
        
        ci::XmlTree init( hrfm::app::App * app, ci::fs::path settingXmlPath );
        
        hrfm::app::App * getApp();
        ci::XmlTree getXml();
        int         getFramerate();
        ivec2       getWindowSize();
        
    private:
        
        friend class hrfm::utils::Singleton<SiAppSettings>;
        
        SiAppSettings():hrfm::utils::DynamicMap(){}
        
        hrfm::app::App * _app;
        ci::XmlTree _xml;
        
    };
    
}}
