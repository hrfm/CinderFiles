#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Font.h"

#include <iostream>
#include <sstream>

using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace std;

namespace sgmnt {
    
    namespace text{
        
        // --- Event for OSC. ---
        
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
            
            // 改行が起こった場合 true
            bool setGlyphByNextText();
            void draw( Rectf bounds );
            void draw( Rectf bounds, float r, float g, float b );
            
            int fontSize;
            
            vector<string>	mFontNames;
            
            Font            mFont;
            TextureFontRef  mTextureFont;
            string          mGlyph;
            
        protected :
            
        private:
            
            string myGlyphs;
            
            int myNumFonts;
            int myLength;
            
            // ログメッセージの表示
            string        myTextStr;
            istringstream myStream;
            string        myCurrentLine;
            int           myCurrentLineIndex;
            
            map<const string, TextureFontRef> myFontMap;
            string myCurrentFontName;
            
        };
        
    }
    
}