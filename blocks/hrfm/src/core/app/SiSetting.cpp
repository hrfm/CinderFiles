#include "SiSetting.h"

using namespace ci;
using namespace hrfm::utils;

namespace hrfm { namespace app{
    
    XmlTree SiSetting::load( ci::fs::path settingXmlPath ){
        _xml = XmlLoader::load( settingXmlPath );
        return _xml;
    }
    
    XmlTree SiSetting::getXmlTree(){
        return _xml;
    }
    
}}