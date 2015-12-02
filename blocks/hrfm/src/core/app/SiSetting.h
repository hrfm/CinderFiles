#pragma once

#include "cinder/Xml.h"
#include "DynamicMap.h"
#include "Singleton.h"
#include "XmlLoader.h"

using namespace ci;

namespace hrfm { namespace app{
    
    class SiSetting : public hrfm::utils::Singleton<SiSetting>, public hrfm::utils::DynamicMap{
        
    public:
        
        XmlTree load( ci::fs::path settingXmlPath );
        
        XmlTree getXmlTree();
        
    private:
        
        friend class hrfm::utils::Singleton<SiSetting>;
        
        SiSetting():hrfm::utils::DynamicMap(){}
        
        XmlTree _xml;
        
    };
    
}}