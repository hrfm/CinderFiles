#pragma once

#include "cinder/app/App.h"
#include "cinder/Xml.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace sgmnt{ namespace utils{
    
    class XmlLoader{
        
    public:
        
        XmlLoader();
        ~XmlLoader();
        
        /*
         
         指定したパスの XML ファイルを読み込みます.
         
         http から始まるファイルパスを指定した場合はネット上から取得します.
         
         ファイル名のみを指定した場合はローカルのファイルを ../resources/ から読み込みます.
         読み込んだローカルのファイルの RootNode に url の指定があった場合そのファイルの取得を試みます.
         
         */
        static XmlTree load( ci::fs::path path );
        static XmlTree load( ci::fs::path path, ci::UrlOptions::UrlOptions urlOptions );
        
    private:
        
    };
    
}}