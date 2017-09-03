#pragma once

#include "cinder/Xml.h"
#include "DataLoader.h"

using namespace ci;
using namespace std;

namespace hrfm{ namespace io{
    
    class XmlLoader{
    
    private:
    
        XmlLoader(){};
        ~XmlLoader(){};
        
    public:
        
        /*
         
         指定したパスの XML ファイルを読み込みます.
         
         http から始まるファイルパスを指定した場合はネット上から取得します.
         
         ファイル名のみを指定した場合はローカルのファイルを Resources ディレクトリから読み込みます.
         読み込んだローカルのファイルの RootNode に url の指定があった場合そのファイルの取得を試みます.
         
         */
        static XmlTree load( ci::fs::path path );
        static XmlTree load( ci::fs::path path, ci::UrlOptions::UrlOptions urlOptions );
        
    private:
        
    };
    
}}
