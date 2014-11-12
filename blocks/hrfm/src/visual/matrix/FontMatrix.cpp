#include "FontMatrix.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace hrfm {
    
    namespace matrix{
        
        // ==== FontMatrix ============================================================
        
        FontMatrix::FontMatrix(){}
        
        void FontMatrix::setup( Vec2i size, int fontSize, const string fontName, string log ){
            
            mySize       = size;
            myFontSize   = fontSize;
            myMatrixSize = Vec2i( ceil( size.x / fontSize ), ceil( size.y / fontSize ) );
            
            myCurrentCol = 0;
            myCurrentRow = 0;
            
            myFontManager.setup( fontSize, "1234567890qwertyuiop[]\\';lkjhgfdsazxcvbnm,./@#$%^&*()_+~<>?:\"{}|" );
            myFontManager.setText(log);
            myFontManager.setFontByName(fontName);
            //myFontManager.setFontByName("misakigothic");
            //myFontManager.setFontByName("OCRAStd");
            
            ci::gl::Fbo::Format format;
            myFboTmp     = ci::gl::Fbo( size.x, size.y, format );
            myFadeFboTmp = ci::gl::Fbo( size.x, size.y, format );
            
            myFbo.setup( size );
            myFadeFbo.setup( size );
            
            mySlideY = 0;
            
        }
        
        int FontMatrix::numCols(){
            return myMatrixSize.x;
        }
        
        int FontMatrix::numRows(){
            return myMatrixSize.y;
        }
        
        void FontMatrix::update(){
            update( 1, true );
        }
        
        void FontMatrix::update( int numUpdateRow, bool random ){
            update( numUpdateRow, random, myMatrixSize.x );
        }
        
        void FontMatrix::setText( string log ){
            myFontManager.setText(log);
        }
        
        
        void FontMatrix::update( int numUpdateRow, bool random, int maxCols ){
            
            int currentCol = myCurrentCol;
            int currentRow = myCurrentRow;
            
            Vec2i windowSize   = getWindowSize();
            Rectf windowBounds = getWindowBounds();
            
            float x;
            float y;
            bool  loop;
            bool  eof;
            vector<int> filled;
            
            if( random == false ){
             
                myFboTmp.bindFramebuffer();
                    ci::gl::pushMatrices();
                    ci::gl::setMatricesWindow(windowSize,false);
                        // draw font.
                        for( int line = 0; line < numUpdateRow; line++ ){
                            loop = true;
                            filled.push_back( currentRow );
                            while( loop ){
                                eof = myFontManager.setGlyphByNextText();
                                x = currentCol * myFontSize;
                                y = currentRow * myFontSize;
                                myFontManager.draw( Rectf(x,y,x+myFontSize,y+myFontSize), 1.0, 1.0, 1.0 );
                                if( eof ){
                                    loop = false;
                                }
                                if( maxCols <= ++currentCol || eof ){
                                    ci::gl::color(0.0,0.0,0.0);
                                    ci::gl::drawSolidRect( Rectf( x+myFontSize, y, windowSize.x, y+myFontSize ) );
                                    currentCol = 0;
                                    if( myMatrixSize.y < ++currentRow ){
                                        currentRow = 0;
                                    }
                                }
                            }
                        }
                    ci::gl::popMatrices();
                myFboTmp.unbindFramebuffer();
                
                mySlideY = (float)currentRow / (float)myMatrixSize.y;
                
                myFadeFboTmp.bindFramebuffer();
                    ci::gl::pushMatrices();
                    ci::gl::setMatricesWindow(windowSize,false);
                        ci::gl::color(0.0f,0.0f,0.0f,0.08f);
                        ci::gl::drawSolidRect(windowBounds);
                        ci::gl::color( 1.0, 1.0, 1.0, 1.0 );
                        vector<int>::iterator it = filled.begin();
                        while( it != filled.end() ){
                            float y = (*it) * myFontSize;
                            ci::gl::drawSolidRect( Rectf( 0, y, windowSize.x, y+myFontSize ) );
                            it++;
                        }
                    ci::gl::popMatrices();
                myFadeFboTmp.unbindFramebuffer();
             
            }else{
                
                myFboTmp.bindFramebuffer();
                    ci::gl::pushMatrices();
                    ci::gl::setMatricesWindow(windowSize,false);
                    // draw font.
                    for( int line = 0; line < numUpdateRow; line++ ){
                        loop = true;
                        filled.push_back( currentCol );
                        while( loop ){
                            myFontManager.setRandomGlyph();
                            x = currentCol * myFontSize;
                            y = currentRow * myFontSize;
                            myFontManager.draw( Rectf(x,y,x+myFontSize,y+myFontSize), 1.0, 1.0, 1.0 );
                            if( myMatrixSize.y < ++currentRow ){
                                currentRow = 0;
                                if( myMatrixSize.x <= ++currentCol ){
                                    currentCol = 0;
                                }
                                loop = false;
                            }
                        }
                    }
                    ci::gl::popMatrices();
                myFboTmp.unbindFramebuffer();
                
                myFadeFboTmp.bindFramebuffer();
                    ci::gl::pushMatrices();
                    ci::gl::setMatricesWindow(windowSize,false);
                        ci::gl::color(0.0f,0.0f,0.0f,0.02f);
                        ci::gl::drawSolidRect(windowBounds);
                        ci::gl::color( 1.0, 1.0, 1.0, 1.0 );
                        vector<int>::iterator it = filled.begin();
                        while( it != filled.end() ){
                            float x = (*it) * myFontSize;
                            ci::gl::drawSolidRect( Rectf( x, 0, x+myFontSize, getWindowHeight() ) );
                            it++;
                        }
                    ci::gl::popMatrices();
                myFadeFboTmp.unbindFramebuffer();
                
            }
            
            // --- 更新.
            
            myCurrentCol = currentCol;
            myCurrentRow = currentRow;
            
            myFbo.setUniform( 0, mySlideY );
            myFbo.affect( myFboTmp.getTexture() );
            
            myFadeFbo.setUniform(0, mySlideY );
            myFadeFbo.affect( myFadeFboTmp.getTexture() );
            
            //*/
            
        }
        
        Texture FontMatrix::getTexture(){
            return myFbo.getTexture();
        }
        
        Texture FontMatrix::getFadeTexture(){
            return myFadeFbo.getTexture();
        }
        
        Rectf FontMatrix::getBounds(){
            return myFboTmp.getBounds();
        }
        
        Vec2i FontMatrix::getSize(){
            return myFboTmp.getSize();
        }
        
    }
    
}