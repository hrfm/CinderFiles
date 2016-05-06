#pragma once

#include "cinder/app/App.h"
#include "cinder/Utilities.h"
#include "DataLoader.h"

using namespace ci;
using namespace std;

namespace hrfm{ namespace io{
    
    class TextureLoader{
        
    private:
        
        TextureLoader(){};
        ~TextureLoader(){};
        
        static map<const string,ci::gl::TextureRef> _cache;
        
    public:
        
        /*
         指定したパスのデータを読み込みます.
         http から始まるファイルパスを指定した場合はネット上から取得します.
         ファイル名のみを指定した場合はローカルのファイルを ../resources/ から読み込みます.
         */
        static ci::gl::TextureRef load( ci::fs::path path, bool useCache = true );
        static ci::gl::TextureRef load( ci::fs::path path, ci::UrlOptions::UrlOptions urlOptions, bool useCache = true );
        
    };
    
}}