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
            
            // 読み込む XML がネット上のファイルだった場合.
            // loadUrl を使いダウンロードします.
            
            cout << endl << "--- XmlLoader::loadXml(" << pathStr << ")" << endl;
            
            try{
                xml = XmlTree( loadUrl( Url( pathStr ) ) );
            }catch(...){
                cout << "Xml [ " << pathStr << " ] is not found." << endl;
                throw;
            }
            
            cout << endl;
            return xml;
            
        }else if( pathStr.find("/") == string::npos ){
            
            // 読み込む XML がファイル名のみの指定だった場合.
            // ../resources/ から読み込みます.
            
            filePath = "../resources/" + pathStr;
            
            pathStr = filePath.native();
            
            cout << "--- XmlLoader::loadXml(" << pathStr << ")" << endl;
            
            try{
                xml = XmlTree( loadResource( pathStr ) );
            }catch(...){
                cout << "Xml [ " << pathStr << " ] is not found." << endl;
                throw;
            }
            
        }else{
            
            // 読み込む XML がファイルパス指定だった場合.
            
            if( pathStr.find("~/") == 0 ){
                pathStr.erase(0,2);
                filePath = getDocumentsDirectory() / ".." / pathStr;
            }else{
                filePath = pathStr;
            }
            
            pathStr = filePath.native();
            
            cout << "--- XmlLoader::loadXml(" << pathStr << ")" << endl;
            
            try{
                xml = XmlTree( loadFile( pathStr ) );
            }catch(...){
                cout << "Xml [ " << pathStr << " ] is not found." << endl;
                throw;
            }
            
        }
        
        XmlTree root = **(xml.getChildren().begin());
        
        if( root.hasAttribute("url") ){
            string url = root.getAttributeValue<string>("url");
            cout << "--- XmlLoader::loadXml(" << url << ")" << endl;
            try{
                xml = XmlTree( loadUrl( Url( url ) ) );
            }catch(...){
                cout << "Xml [ " << url << " ] is not found. Use local xml file." << endl;
            }
        }
        
        cout << endl;
        
        return xml;
        
    }
    
}}