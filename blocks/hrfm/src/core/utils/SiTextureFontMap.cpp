#include "SiTextureFontMap.h"

namespace hrfm{ namespace utils{
    
    ci::gl::TextureFontRef SiTextureFontMap::getTextureFontRef( string fontName, int fontSize ){
        
        string key = fontName + "_" + to_string(fontSize);
        
        if( this->find(key) == this->end() ){
            this->insert( map<string,ci::gl::TextureFontRef>::value_type( key, ci::gl::TextureFont::create( ci::Font(fontName,fontSize) ) ) );
        }
        
        return this->at(key);
        
    }
    
}}