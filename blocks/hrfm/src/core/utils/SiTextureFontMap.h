#pragma once

#include <map>
#include "Singleton.h"
#include "cinder/Font.h"
#include "cinder/gl/TextureFont.h"

using namespace std;

namespace hrfm{ namespace utils{
    
    /*
     
     std::map を拡張し Singleton で何処からでも特定の TextureFont のインスタンスにアクセス出来るようにするクラスです.
     
     */
    class SiTextureFontMap : public Singleton<SiTextureFontMap>, public std::map<string, ci::gl::TextureFontRef>{
        
    public:
        
        virtual ci::gl::TextureFontRef getTextureFontRef( string fontName, int fontSize );
        
    private:
        
        friend class Singleton<SiTextureFontMap>;
        
        SiTextureFontMap(){
            std::map<string,ci::gl::TextureFontRef>();
        }
        
    };
    
}}