#pragma once

#include "SiDisplayUtil.h"
#include "SiFontUtil.h"
#include "SiTextureMap.h"
#include "SiTimeUtil.h"
#include "SiVolumeDetector.h"
#include "XmlLoader.h"

namespace hrfm{ namespace utils{
    
    static int GCD( int a, int b ){
        int c;
        if (a < b) {
            a += b;
            b =  a-b;
            a -= b;
        }
        while (b != 0) {
            c = a % b;
            a = b;
            b = c;
        }
        return a;
    }
    
    static ci::Vec2i getAspectRatio( ci::Vec2i resolution ){
        return resolution / GCD( resolution.x, resolution.y );
    }
    
    static float sigmoid( float value, float gain ){
        return 1.0 / ( 1.0 + pow( M_E, - gain * value ) );
    }
    
    static fs::path generateFilePath( string pathStr ){
        fs::path filePath;
        if( pathStr.find("/") == string::npos ){
            // ファイル名のみの指定だった場合.
            // ../resources/ から読み込みます.
            filePath = "../resources/" + pathStr;
        }else if( pathStr.find("~/") == 0 ){
            // User Directory のパスが指定されていた場合.
            pathStr.erase(0,2);
            filePath = getDocumentsDirectory() / ".." / pathStr;
        }else{
            filePath = pathStr;
        }
        return filePath;
    }
    
}}