#include "DataLoader.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace hrfm{ namespace utils{
    
    DataSourceRef DataLoader::load( ci::fs::path filePath ){
        return DataLoader::load( filePath, ci::UrlOptions::UrlOptions( true, 30.0f ) );
    }
    
    DataSourceRef DataLoader::load( ci::fs::path filePath, ci::UrlOptions::UrlOptions urlOptions ){
        
        string pathStr = filePath.native();
        
        if( pathStr.find("http") == 0 ){
            
            // 読み込む Data がネット上のファイルだった場合.
            // loadUrl を使いダウンロードします.
            
            cout << endl << "DataLoader::loadUrl(" << pathStr << ")" << endl;
            
            try{
                return loadUrl( Url( pathStr ) );
            }catch(...){
                cout << "Data [ " << pathStr << " ] is not found." << endl;
                throw;
            }
            
            cout << endl;
            
        }else if( pathStr.find("/") == string::npos ){
            
            // 読み込む Data がファイル名のみの指定だった場合.
            // ../resources/ から読み込みます.
            
            filePath = "../resources/" + pathStr;
            
            pathStr = filePath.native();
            
            cout << "DataLoader::loadResource(" << pathStr << ")" << endl;
            
            try{
                return loadResource( pathStr );
            }catch(...){
                cout << "Data [ " << pathStr << " ] is not found." << endl;
                throw;
            }
            
        }else{
            
            // 読み込む Data がファイルパス指定だった場合.
            
            if( pathStr.find("~/") == 0 ){
                pathStr.erase(0,2);
                filePath = getDocumentsDirectory() / ".." / pathStr;
            }else{
                filePath = pathStr;
            }
            
            pathStr = filePath.native();
            
            cout << "DataLoader::loadFile(" << pathStr << ")" << endl;
            
            try{
                return loadFile( pathStr );
            }catch(...){
                cout << "Data [ " << pathStr << " ] is not found." << endl;
                throw;
            }
            
        }
        
        return NULL;
        
    }
    
}}