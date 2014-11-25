#pragma once

// === INCLUDE ====================================================================================================

#include <iostream>
#include <sstream>
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Font.h"
#include "SiFontUtil.h"

// === NAMESPACE ===================================================================================================

using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace std;

// === FontManager =================================================================================================

namespace hrfm{ namespace text{
    
    class FontManager{
        
    public:
        
        FontManager();
        
        void setup( int size );
        void setup( int size, string glyphs );
        
        void setText( string text );
        
        void setFont( int index );
        void setFontByName( const string fontName );
        void setRandomFont();
        void setRandomGlyph();
        
        Vec2f measureString( string txt );
        
        // 改行が起こった場合 true
        string setGlyphByNextText();
        bool isLineHead();
        
        void draw( Rectf bounds );
        void draw( string text, Rectf bounds );
        
        void draw( Rectf bounds, float r, float g, float b );
        void draw( string text, Rectf bounds, float r, float g, float b );
        
        vector<string>	mFontNames;
        
        Font            mFont;
        TextureFontRef  mTextureFont;
        
    private:
        
        string myGlyphs;
        
        int myNumFonts;
        int myLength;
        
        // ログメッセージの表示
        string        myTextStr;
        istringstream myStream;
        string        myCurrentLine;
        int           myCurrentLineIndex;
        
        string _glyph;
        string _glyphs;
        
        vector<string> _fontNames;
        int    _fontSize;
        TextureFontRef _textureFontRef;
        
    };
    
}}