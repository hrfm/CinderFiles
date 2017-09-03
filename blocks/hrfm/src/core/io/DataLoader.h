#pragma once

#include "cinder/app/App.h"
#include "cinder/Utilities.h"

using namespace ci;
using namespace std;

namespace hrfm{ namespace io{
    
    class DataLoader{
    
    private:
        
        DataLoader(){};
        ~DataLoader(){};
        
    public:
        
        /*
         指定したパスのデータを読み込みます.
         http から始まるファイルパスを指定した場合はネット上から取得します.
         ファイル名のみを指定した場合はローカルのファイルを Resources ディレクトリから読み込みます.
         */
        static DataSourceRef load( ci::fs::path path );
        static DataSourceRef load( ci::fs::path path, ci::UrlOptions::UrlOptions urlOptions );
        
    private:
        
    };
    
}}
