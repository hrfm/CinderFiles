#include "SiFontUtil.h"

using namespace ci;
using namespace hrfm::utils;

namespace hrfm{ namespace text{
    
    void SiFontUtil::showAllFontNames(){
        for( vector<string>::const_iterator fontName = _fontNames.begin(); fontName != _fontNames.end(); ++fontName ) {
            cout << *fontName << endl;
        }
    }
    
    vector<string> SiFontUtil::getFontNames(){
        return _fontNames;
    }
    
    string SiFontUtil::getFontNameRandomly(){
        return _fontNames[rand() % _fontNames.size()];
    }
    
    ci::gl::TextureFontRef SiFontUtil::getTextureFontRef( string fontName, int fontSize ){
        
        string key = fontName + "_" + to_string(fontSize);
        
        auto format = ci::gl::TextureFont::Format();
        
        if( _map.find(key) == _map.end() ){
            if( fontName.find(".") == string::npos ){
                _map.insert( map<string,ci::gl::TextureFontRef>::value_type(
                    key, ci::gl::TextureFont::create( ci::Font( fontName, fontSize ), format )
                ) );
            }else{
                _map.insert( map<string,ci::gl::TextureFontRef>::value_type(
                    key, ci::gl::TextureFont::create(
                        ci::Font( io::DataLoader::load(fontName), fontSize ), format
                    )
                ) );
            }
        }
        
        return _map.at(key);
        
    }
    
}}