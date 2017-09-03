#pragma once

#include <map>
#include "DataLoader.h"
#include "Singleton.h"
#include "cinder/Font.h"
#include "cinder/gl/TextureFont.h"

using namespace ci;
using namespace std;
using namespace hrfm::utils;

namespace hrfm{ namespace utils{
    
    /*
     
     std::map を拡張し Singleton で何処からでも特定の TextureFont のインスタンスにアクセス出来るようにするクラスです.
     
     */
    class SiFontUtil : public Singleton<SiFontUtil>{
        
    public:
        
        //! cout all installed fonts name.
        virtual void showAllFontNames();
        
        //! Get font name list
        virtual vector<string> getFontNames();
        
        //! Get font name randomly from font list.
        virtual string getFontNameRandomly();
        
        virtual ci::gl::TextureFontRef getTextureFontRef( string fontName, int fontSize );
        
    private:
        
        friend class Singleton<SiFontUtil>;
        
        SiFontUtil(){
            _fontNames = Font::getNames();
        }
        
        vector<string> _fontNames;
        std::map<string,ci::gl::TextureFontRef> _map;
        
    };
    
}}
