#include "FontManager.h"

using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace std;

namespace hrfm { namespace utils{
    
    
    FontManager::FontManager(){};
    
    void FontManager::setup( int size ){
        setup( size, string("1234567890qwertyuiop[]\\';lkjhgfdsazxcvbnm,./@#$%^&*()_+~<>?:\"{}|  ") );
    }
    
    void FontManager::setup( int size, string glyphs ){
        
        myGlyphs = glyphs;
        
        fontSize = size;
        mFontNames = Font::getNames();
        myNumFonts = mFontNames.size();
        
        setText(" ");
        
        setRandomFont();
        setRandomGlyph();
        
        return;
        
        for( vector<string>::const_iterator fontName = mFontNames.begin(); fontName != mFontNames.end(); ++fontName ) {
            cout << *fontName << endl;
        }
        
    }
    
    void FontManager::setText( string text ){
        myTextStr = text;
        myStream.str( myTextStr );
        getline( myStream, myCurrentLine );
        myCurrentLineIndex = 0;
    }
    
    void FontManager::setFont( int index ){
        try{
            string fontName = mFontNames[index];
            if( !myFontMap[fontName] ){
                Font font = Font( fontName, fontSize );
                myFontMap[fontName] = gl::TextureFont::create(font);
            }
            myCurrentFontName = fontName;
        }catch(...){}
    }
    
    void FontManager::setFontByName( const string fontName ){
        try{
            if( !myFontMap[fontName] ){
                Font font;
                if( fontName == "misakigothic" ){
                    font = Font( ci::app::loadResource("../resources/misakigothic.ttf"), fontSize );
                }else{
                    font = Font( fontName, fontSize );
                }
                myFontMap[fontName] = gl::TextureFont::create(font);
            }
            myCurrentFontName = fontName;
        }catch(...){}
    }
    
    void FontManager::setRandomFont(){
        try{
            string fontName = mFontNames[rand() % myNumFonts];
            if( !myFontMap[fontName] ){
                Font font = Font( fontName, fontSize );
                myFontMap[fontName] = gl::TextureFont::create(font);
            }
            myCurrentFontName = fontName;
        }catch(...){}
    }
    
    void FontManager::setRandomGlyph(){
        try{
            int pos = rand()%myGlyphs.size();
            unsigned char lead = myGlyphs[pos];
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
            mGlyph = myGlyphs.substr( pos, char_size );
        }catch(...){
            mGlyph = "0";
        }
    }
    
    // 改行が起こった場合 true
    bool FontManager::setGlyphByNextText(){
        if( myCurrentLine.length() == 0 ){
            if( !getline( myStream, myCurrentLine ) ){
                myStream.clear();
                myStream.str( myTextStr );
                getline( myStream, myCurrentLine );
            }
            myCurrentLineIndex = 0;
            return true;
        }
        char c = myCurrentLine.at( myCurrentLineIndex );
        mGlyph = c;
        if( myCurrentLine.length() <= ++myCurrentLineIndex ){
            if( !getline( myStream, myCurrentLine ) ){
                myStream.clear();
                myStream.str( myTextStr );
                getline( myStream, myCurrentLine );
            }
            myCurrentLineIndex = 0;
            return true;
        }else{
            return false;
        }
    }
    
    void FontManager::draw( Rectf bounds ){
        draw( bounds, 1.0f, 1.0f, 1.0f );
    }
    
    void FontManager::draw( Rectf bounds, float r, float g, float b ){
        gl::color(0.0f,0.0f,0.0f);
        gl::drawSolidRect( bounds );
        gl::color( r, g, b );
        try{
            myFontMap[myCurrentFontName]->drawString( mGlyph, bounds );
            //Vec2f(bounds.x1+(fontSize*(1/3)),bounds.y1+(fontSize*0.8)));
        }catch(...){
            cout << "error" << endl;
        }
    }
    
}}