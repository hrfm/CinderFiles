#include "FontMatrix.h"
#include <random>

using namespace ci;
using namespace ci::app;
using namespace std;

namespace hrfm{ namespace text{
    
    FontMatrix::FontMatrix(){
        hrfm::display::DisplayNode();
        _currentCol = 0;
        _currentRow = 0;
        mySlideY    = 0;
    }
    
    void FontMatrix::setup( Vec2i fboSize, int fontSize, const string fontName, string text ){
        
        _fboSize  = fboSize;
        _fontSize = fontSize;
        _text     = text;
        
        // --- Setup using Charactors and TextureFont.
        
        string charList = "1234567890qwertyuiop[]\\';lkjhgfdsazxcvbnm,./@#$%^&*()_+~<>?:\"{}|";
        
        myFontManager.setup( fontSize, charList );
        myFontManager.setFontByName(fontName);
        myFontManager.setText(text);
        
        // --- Measure num of matrix cols.
        
        string txt = "0";
        int length = 0;
        while( myFontManager.measureString(txt).x < _fboSize.x ){
            txt += "0";
            length++;
        }
        _mtxSize  = Vec2i( length, ceil( _fboSize.y / fontSize ) );
        
        // --- Generate random text.
        
        std::random_device rnd;
        std::mt19937 mt(rnd());
        std::uniform_real_distribution<float> rndm(0.0,1.0);
        for( int i = 0; i < 1000; i++ ){
            string txt = "";
            for( int j = 0; j < _mtxSize.x; j++ ){
                txt += charList[floor(rndm(mt)*charList.size())];
            }
            _randomTextList.push_back(txt);
        }
        
        // --- Create FBO.
        
        ci::gl::Fbo::Format format;
        _fbo = ci::gl::Fbo( _fboSize.x, _fboSize.y, format );
        myFadeFboTmp = ci::gl::Fbo( _fboSize.x, _fboSize.y, format );
        
        myFbo.setup( _fboSize );
        myFadeFbo.setup( _fboSize );
        
    }
    
    int FontMatrix::numMatrixCols(){
        return _mtxSize.x;
    }
    
    int FontMatrix::numMatrixRows(){
        return _mtxSize.y;
    }
    
    Vec2i FontMatrix::getMatrixSize(){
        return _mtxSize;
    }
    
    Vec2i FontMatrix::getFboSize(){
        return _fbo.getSize();
    }
    
    Rectf FontMatrix::getFboBounds(){
        return _fbo.getBounds();
    }
    
    void FontMatrix::setText( string text ){
        _text = text;
        myFontManager.setText(text);
    }
    
    Texture FontMatrix::getTexture(){
        return myFbo.getTexture();
    }
    
    Texture FontMatrix::getFadeTexture(){
        return myFadeFbo.getTexture();
    }
    
    void FontMatrix::scroll( int numScroll ){
        
        int    line, y;
        bool   loop;
        string text;
        vector<int> filled;
        
        int col = _currentCol;
        int row = _currentRow;
        
        Rectf fboBounds = getFboBounds();
        Area viewport   = ci::gl::getViewport();
        
        // -----
        
        _fbo.bindFramebuffer();
        {
            ci::gl::pushMatrices();
            ci::gl::setViewport( (Area)fboBounds );
            ci::gl::setMatricesWindow( _fboSize, false );
            {
                // draw font.
                for( line = 0; line < numScroll; line++ ){
                    loop   = true;
                    text   = "";
                    filled.push_back( col );
                    y = row * _fontSize;
                    while( loop ){
                        text += myFontManager.setGlyphByNextText();
                        if( _fboSize.x <= ++col * (_fontSize*0.7) || myFontManager.isLineHead() ){
                            col = 0;
                            if( _mtxSize.y < ++row ){
                                row = 0;
                            }
                            loop = false;
                        }
                    }
                    myFontManager.draw( text, Rectf( 0, y, _fboSize.x, y+_fontSize ), 1.0, 1.0, 1.0 );
                }
            }
            ci::gl::popMatrices();
        }
        _fbo.unbindFramebuffer();
        
        mySlideY = (float)row / (float)_mtxSize.y;
        
        myFadeFboTmp.bindFramebuffer();
        {
            ci::gl::pushMatrices();
            ci::gl::setViewport( (Area)fboBounds );
            ci::gl::setMatricesWindow( _fboSize, false );
            {
                // draw
                ci::gl::color(0.0f,0.0f,0.0f,0.08f);
                ci::gl::drawSolidRect(fboBounds);
                ci::gl::color( 1.0, 1.0, 1.0, 1.0 );
                vector<int>::iterator it = filled.begin();
                while( it != filled.end() ){
                    float y = (*it) * _fontSize;
                    ci::gl::drawSolidRect( Rectf( 0, y, _fboSize.x, y+_fontSize ) );
                    it++;
                }
            }
            ci::gl::popMatrices();
        }
        myFadeFboTmp.unbindFramebuffer();
        
        ci::gl::setViewport( viewport );
        
        // --- update properties.
        
        _currentCol = col;
        _currentRow = row;
        
        myFbo.setUniform( 0, mySlideY );
        myFbo.affect( _fbo.getTexture() );
        
        myFadeFbo.setUniform(0, mySlideY );
        myFadeFbo.affect( myFadeFboTmp.getTexture() );
        
    }
    
    void FontMatrix::shuffle(){
        
        Rectf fboBounds = getFboBounds();
        Area viewport   = ci::gl::getViewport();
        
        // -----
        
        std::random_device rnd;
        std::mt19937 mt(rnd());
        std::uniform_int_distribution<int> rndm(0,99);
        
        _fbo.bindFramebuffer();
        {
            ci::gl::pushMatrices();
            ci::gl::setViewport( (Area)fboBounds );
            ci::gl::setMatricesWindow( _fboSize.x, _fboSize.y, false );
            {
                // draw font.
                for( int line = 0; line < _mtxSize.y; line++ ){
                    int y = line * _fontSize;
                    myFontManager.draw( _randomTextList.at( rndm(mt) ), Rectf( 0, y, _fboSize.x, y+_fontSize ), 1.0, 1.0, 1.0 );
                }
            }
            ci::gl::popMatrices();
        }
        _fbo.unbindFramebuffer();
        
        ci::gl::setViewport(viewport);
        
        _currentCol = 0;
        _currentRow = 0;
        mySlideY    = 0;
        
        myFbo.setUniform( 0, mySlideY );
        myFbo.affect( _fbo.getTexture() );
        
    }
    
    
    
    
    
    
    
    
    void FontMatrix::update( int numUpdateRow, bool random ){
        update( numUpdateRow, random, _mtxSize.x );
    }
    
    void FontMatrix::update( int numUpdateRow, bool random, int maxCols ){
        if( random == false ){
            
            scroll( numUpdateRow );
            
        }else{
            
            int currentCol = _currentCol;
            int currentRow = _currentRow;
            
            Area viewport  = ci::gl::getViewport();
            Rectf bounds   = getFboBounds();
            
            float x;
            float y;
            bool  loop;
            string text;
            vector<int> filled;
            
            _fbo.bindFramebuffer();
            {
                ci::gl::pushMatrices();
                ci::gl::setViewport( (Area)bounds );
                ci::gl::setMatricesWindow( _fboSize.x, _fboSize.y, false );
                {
                    // draw font.
                    for( int line = 0; line < numUpdateRow; line++ ){
                        loop = true;
                        filled.push_back( currentCol );
                        while( loop ){
                            myFontManager.setRandomGlyph();
                            x = currentCol * _fontSize;
                            y = currentRow * _fontSize;
                            myFontManager.draw( Rectf(x,y,x+_fontSize,y+_fontSize), 1.0, 1.0, 1.0 );
                            if( _mtxSize.y < ++currentRow ){
                                currentRow = 0;
                                if( _mtxSize.x <= ++currentCol ){
                                    currentCol = 0;
                                }
                                loop = false;
                            }
                        }
                    }
                }
                ci::gl::popMatrices();
            }
            _fbo.unbindFramebuffer();
            
            myFadeFboTmp.bindFramebuffer();
            {
                ci::gl::pushMatrices();
                ci::gl::setViewport( (Area)bounds );
                ci::gl::setMatricesWindow( _fboSize.x, _fboSize.y, false );
                {
                    ci::gl::color(0.0f,0.0f,0.0f,0.02f);
                    ci::gl::drawSolidRect(bounds);
                    ci::gl::color( 1.0, 1.0, 1.0, 1.0 );
                    vector<int>::iterator it = filled.begin();
                    while( it != filled.end() ){
                        float x = (*it) * _fontSize;
                        ci::gl::drawSolidRect( Rectf( x, 0, x+_fontSize, getWindowHeight() ) );
                        it++;
                    }
                }
                ci::gl::popMatrices();
            }
            myFadeFboTmp.unbindFramebuffer();
            
            // --- 更新.
            
            _currentCol = currentCol;
            _currentRow = currentRow;
            
            myFbo.setUniform( 0, mySlideY );
            myFbo.affect( _fbo.getTexture() );
            
            myFadeFbo.setUniform(0, mySlideY );
            myFadeFbo.affect( myFadeFboTmp.getTexture() );
            
            //*/
            
            ci::gl::setViewport(viewport);
            
        }
    }
    
    //! protected;
    
    void FontMatrix::_update(){
        if( _text != "" ){
            scroll();
        }else{
            shuffle();
        }
    }
    
    void FontMatrix::_draw(){
        ci::gl::enableAdditiveBlending();
        ci::gl::draw( myFbo.getTexture(), getDrawBounds() );
        ci::gl::disableAlphaBlending();
    }
    
}}