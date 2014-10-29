#include "XmlLoader.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace sgmnt{ namespace utils{
    
    XmlLoader::XmlLoader(){}
    XmlLoader::~XmlLoader(){}
    
    XmlTree XmlLoader::load( ci::fs::path filePath ){
        
        XmlTree xml;
        string pathStr = filePath.native();
        
        if( pathStr.find("http") == 0 ){
            cout << endl << "--- XmlLoader::loadXml(" << pathStr << ")" << endl;
            try{
                xml = XmlTree( loadUrl( Url( pathStr ) ) );
            }catch(...){
                cout << "Xml [ " << pathStr << " ] is not found." << endl;
                throw;
            }
        }else{
            
            if( pathStr.find("~/") == 0 ){
                pathStr.erase(0,2);
                filePath = getDocumentsDirectory() / ".." / pathStr;
            }else if( pathStr.find("/") == string::npos ){
                filePath = "../resources/" + pathStr;
            }else{
                filePath = pathStr;
            }
            
            pathStr = filePath.native();
            
            cout << "--- XmlLoader::loadXml(" << pathStr << ")" << endl;
            
            try{
                xml = XmlTree( loadResource( pathStr ) );
            }catch(...){
                cout << "Xml [ " << pathStr << " ] is not found." << endl;
                throw;
            }
            
            XmlTree root = **(xml.getChildren().begin());
            
            if( root.hasAttribute("url") ){
                string url = root.getAttributeValue<string>("url");
                cout << "--- XmlLoader::loadXml(" << url << ")" << endl;
                try{
                    xml = XmlTree( loadUrl( Url( url ) ) );
                }catch(...){
                    cout << "Xml [ " << url << " ] is not found." << endl;
                }
            }
            
        }
        
        cout << endl;
        
        return xml;
        
    }
    
}}