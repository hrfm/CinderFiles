#include "XmlLoader.h"

using namespace ci;
using namespace std;

namespace hrfm{ namespace io{
    
    XmlTree XmlLoader::load( ci::fs::path filePath ){
        return XmlLoader::load( filePath, ci::UrlOptions::UrlOptions( true, 30.0f ) );
    }
    
    XmlTree XmlLoader::load( ci::fs::path filePath, ci::UrlOptions::UrlOptions urlOptions ){
        
        string pathStr = filePath.native();
        
        if( pathStr.find("http") == 0 ){
            return XmlTree( DataLoader::load( filePath, urlOptions ) );
        }
        
        XmlTree xml = XmlTree( DataLoader::load( filePath, urlOptions ) );
        XmlTree root = **(xml.getChildren().begin());
        
        if( root.hasAttribute("url") ){
            string url = root.getAttributeValue<string>("url");
            cout << "XmlLoader::load(" << url << ")" << endl;
            try{
                xml = XmlTree( loadUrl( Url( url ), urlOptions ) );
            }catch(...){
                cout << "Xml [ " << url << " ] is not found. Use local xml file." << endl;
            }
        }
        
        cout << xml;
        
        return xml;
        
    }
    
}}