#include "FontManager.h"

using namespace ci;
using namespace ci::gl;
using namespace std;
using namespace hrfm::utils;

namespace hrfm { namespace text{
    
    
    FontManager::FontManager(){};
    
    void FontManager::setup( int size ){
        setup( size, string("1234567890qwertyuiop[]\\';lkjhgfdsazxcvbnm,./@#$%^&*()_+~<>?:\"{}|  ") );
    }
    
    void FontManager::setup( int size, string glyphs ){
        _fontSize = size;
        _glyphs   = glyphs;
        setText(" ");
        setRandomFont();
        setRandomGlyph();
    }
    
    void FontManager::setText( string text ){
        myTextStr = text;
        myStream.str( myTextStr );
        getline( myStream, myCurrentLine );
        myCurrentLineIndex = 0;
    }
    
    vec2 FontManager::measureString( string text ){
        return _textureFontRef->measureString(text);
    }
    
    void FontManager::setFont( int index ){
        try{
            _textureFontRef = SiFontUtil::getInstance().getTextureFontRef( _fontNames[index], _fontSize );
        }catch(...){}
    }
    
    void FontManager::setFontByName( const string fontName ){
        try{
            if( fontName == "misakigothic" ){
                _textureFontRef = SiFontUtil::getInstance().getTextureFontRef( "misakigothic.ttf", _fontSize );
            }else{
                _textureFontRef = SiFontUtil::getInstance().getTextureFontRef( fontName, _fontSize );
            }
        }catch(...){}
    }
    
    void FontManager::setRandomFont(){
        try{
            _textureFontRef = SiFontUtil::getInstance().getTextureFontRef( SiFontUtil::getInstance().getFontNameRandomly(), _fontSize );
        }catch(...){}
    }
    
    void FontManager::setRandomGlyph(){
        try{
            int pos = rand()%_glyphs.size();
            unsigned char lead = _glyphs[pos];
            int char_size;
            if (lead < 0x80) {
                char_size = 1;
            } else if (lead < 0xE0) {
                char_size = 2;
            } else if (lead < 0xF0) {
                char_size = 3;
            } else {
                char_size = 4;
            }
            _glyph = _glyphs.substr( pos, char_size );
        }catch(...){
            _glyph = "0";
        }
    }
    
    // 改行が起こった場合 true
    string FontManager::setGlyphByNextText(){
        
        if( myCurrentLine.length() == 0 ){
            if( !getline( myStream, myCurrentLine ) ){
                myStream.clear();
                myStream.str( myTextStr );
                getline( myStream, myCurrentLine );
            }
            myCurrentLineIndex = 0;
            return "";
        }
        
        char c = myCurrentLine.at( myCurrentLineIndex );
        _glyph = c;
        
        if( myCurrentLine.length() <= ++myCurrentLineIndex ){
            if( !getline( myStream, myCurrentLine ) ){
                myStream.clear();
                myStream.str( myTextStr );
                getline( myStream, myCurrentLine );
            }
            myCurrentLineIndex = 0;
        }
        
        return _glyph;
        
    }
    
    bool FontManager::isLineHead(){
        return myCurrentLineIndex == 0;
    }
    
    void FontManager::draw( Rectf bounds, float r, float g, float b ){
        draw( _glyph, bounds, r, g, b );
    }
    
    void FontManager::draw( string text, Rectf bounds, float r, float g, float b ){
        
        ci::gl::color(0.0f,0.0f,0.0f,1.0f);
        ci::gl::drawSolidRect( bounds );
        
        ci::gl::color( r, g, b, 1.0f );
        try{
            _textureFontRef->drawString( text, bounds );
            //vec2(bounds.x1+(fontSize*(1/3)),bounds.y1+(fontSize*0.8)));
        }catch(...){
            cout << "error" << endl;
        }
        
    }
    
}}