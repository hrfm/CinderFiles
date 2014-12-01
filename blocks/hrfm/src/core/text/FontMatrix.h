#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "DisplayNode.h"
#include "FontManager.h"
#include "FilterHeaders.h"
#include "OutputEffect.h"

using namespace ci;
using namespace ci::gl;
using namespace std;
using namespace hrfm::text;
using namespace hrfm::utils;
using namespace hrfm::gl::filter;

namespace hrfm{ namespace text{
    
    class FontMatrix : public hrfm::display::DisplayNode{
        
    public:
        
        FontMatrix();
        
        void setup( Vec2i fboSize, int fontSize, const string fontName, string text = "" );
        
        int   numMatrixCols();
        int   numMatrixRows();
        Vec2i getMatrixSize();
        
        Vec2i getFboSize();
        Rectf getFboBounds();
        
        ci::gl::Texture getTexture();
        ci::gl::Texture getFadeTexture();
        
        void setText( string text );
        
        //! setText で指定した内容を引数で指定した行数ずつ表示します.
        void scroll( int numScroll = 1 );
        
        //! Matrix のすべての文字をランダムに書き換えます.
        void shuffle();
        
        void update( int numUpdateFont, bool random );
        void update( int numUpdateFont, bool random, int maxCols );
    
    protected:
        
        virtual void _update();
        virtual void _draw();
        
    private:
        
        string _text;
        float _measureX;
        
        Vec2i _fboSize;
        int   _fontSize;
        Vec2i _mtxSize;
        int   _currentCol;
        int   _currentRow;
        
        ci::gl::Fbo _fbo;
        
        vector<string> _randomTextList;
        
        FontManager myFontManager;
        
        
        
        ci::gl::Fbo myFboTmp;
        ci::gl::Fbo myFadeFboTmp;
        
        ScrollFilter myFbo;
        ScrollFilter myFadeFbo;
        
        float mySlideY;
        
    };
    
}}