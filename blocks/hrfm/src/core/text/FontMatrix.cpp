#include "FontMatrix.h"
#include <random>

using namespace ci;
using namespace std;

namespace hrfm{ namespace text{
    
    FontMatrix::FontMatrix(){
        hrfm::display::DisplayNode();
        _currentCol = 0;
        _currentRow = 0;
        mySlideY    = 0;
    }
    
    void FontMatrix::setup( ivec2 fboSize, int fontSize, const string fontName, string text ){
        
        _fboSize  = fboSize;
        _fontSize = fontSize;
        _text     = text;
        
        
        cout << fboSize << ":" << fontSize << endl;
        
        // --- Setup using Charactors and TextureFont.
        
        string charList = "1234567890qwertyuiop[]\\';lkjhgfdsazxcvbnm,./@#$%^&*()_+~<>?:\"{}| ";
        
        myFontManager.setup( fontSize, charList );
        myFontManager.setFontByName(fontName);
        myFontManager.setText(text);
        
        // --- Measure num of matrix cols.
        
        string txt = "";
        int length = 0;
        
        while( true ){
            txt += "0";
            float measureX = myFontManager.measureString(txt).x;
            if( measureX < _fboSize.x ){
                _measureX = measureX;
                length++;
            }else{
                break;
            }
        }
        _fontRealWidth = _measureX / length;
        
        _mtxSize  = ivec2( length, ceil( _fboSize.y / fontSize ) );
        
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
        
        fbo     = new hrfm::gl::ExFbo(_fboSize.x, _fboSize.y);
        fadeFbo = new hrfm::gl::ExFbo(_fboSize.x, _fboSize.y);
        scrollFilter.setup( _fboSize );
        
    }
    
    int FontMatrix::numMatrixCols(){
        return _mtxSize.x;
    }
    
    int FontMatrix::numMatrixRows(){
        return _mtxSize.y;
    }
    
    ivec2 FontMatrix::getMatrixSize(){
        return _mtxSize;
    }
    
    ivec2 FontMatrix::getFboSize(){
        return fbo->getSize();
    }
    
    Rectf FontMatrix::getFboBounds(){
        return fbo->getBounds();
    }
    
    void FontMatrix::setText( string text ){
        _text = text;
        myFontManager.setText(text);
    }
    
    TextureRef FontMatrix::getTexture(){
        return fbo->getTexture();
    }
    
    TextureRef FontMatrix::getFadeTexture(){
        return fadeFbo->getTexture();
    }
    
    void FontMatrix::scroll( int numScroll ){
        
        int    line, y;
        bool   loop;
        string text;
        vector<int> filled;
        
        int col = _currentCol;
        int row = _currentRow;
        
        // -----
        
        scrollFilter.setUniform( 0, - ( (float)numScroll / (float)_mtxSize.y ) );
        
        // -----
        
        fbo->beginOffscreen()->applyFilter(&scrollFilter);
        // draw font.
        for( line = 0; line < numScroll; line++ ){
            loop = true;
            text = "";
            y = ( _mtxSize.y - numScroll + line ) * _fontSize;
            filled.push_back( y );
            while( loop ){
                text += myFontManager.setGlyphByNextText();
                if( _fboSize.x <= (++col+1) * _fontRealWidth || myFontManager.isLineHead() ){
                    col = 0;
                    if( _mtxSize.y < ++row ){
                        row = 0;
                    }
                    loop = false;
                }
            }
            myFontManager.draw( text, Rectf( 0, y, _fboSize.x, y+_fontSize ), 1.0, 1.0, 1.0 );
        }
        _currentCol = col;
        _currentRow = row;
        fbo->endOffscreen();
        
    }
    
    void FontMatrix::shuffle(){
        
        std::random_device rnd;
        std::mt19937 mt(rnd());
        std::uniform_int_distribution<int> rndm(0,99);
        
        fbo->beginOffscreen();
        {
            // draw font.
            for( int line = 0; line < _mtxSize.y; line++ ){
                int y = line * _fontSize;
                myFontManager.draw( _randomTextList.at( rndm(mt) ), Rectf( 0, y, _fboSize.x, y+_fontSize ), 1.0, 1.0, 1.0 );
            }
            _currentCol = 0;
            _currentRow = 0;
            mySlideY    = 0;
        }
        fbo->endOffscreen();
        
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
            
            float x;
            float y;
            bool  loop;
            string text;
            vector<int> filled;
            
            fbo->beginOffscreen();
            {
                // draw font.
                for( int line = 0; line < numUpdateRow; line++ ){
                    loop = true;
                    filled.push_back( currentCol );
                    while( loop ){
                        myFontManager.setRandomGlyph();
                        x = currentCol * _fontRealWidth;
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
                _currentCol = currentCol;
                _currentRow = currentRow;
            }
            fbo->endOffscreen();
            
            fadeFbo->beginOffscreen();
            {
                ci::gl::color(0.0f,0.0f,0.0f,0.02f);
                ci::gl::drawSolidRect(getFboBounds());
                ci::gl::color( 1.0, 1.0, 1.0, 1.0 );
                vector<int>::iterator it = filled.begin();
                while( it != filled.end() ){
                    float x = (*it) * _fontSize;
                    ci::gl::drawSolidRect( Rectf( x, 0, x+_fontSize, ci::app::getWindowHeight() ) );
                    it++;
                }
            }
            fadeFbo->endOffscreen();
            
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
        //ci::gl::enableAdditiveBlending();
        ci::gl::draw( fbo->getTexture(), getDrawBounds() );
        //ci::gl::disableAlphaBlending();
    }
    
}}