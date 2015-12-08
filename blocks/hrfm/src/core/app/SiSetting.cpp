#include "SiSetting.h"

using namespace ci;

namespace hrfm { namespace app{
    
    XmlTree SiSetting::load( ci::fs::path settingXmlPath ){
        _xml = hrfm::io::XmlLoader::load( settingXmlPath );
        return _xml;
    }
    
    XmlTree SiSetting::getXmlTree(){
        return _xml;
    }
    
}}