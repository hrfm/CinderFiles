#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "FontManager.h"
#include "OutputEffect.h"

using namespace ci;
using namespace ci::gl;
using namespace std;
using namespace hrfm::utils;

namespace hrfm {
    
    namespace matrix{
        
        class FontMatrix{
            
        public:
            
            FontMatrix();
            
            void setup( Vec2i size, int fontSize, const string fontName, string log );
            void update();
            void update( int numUpdateFont, bool random );
            void update( int numUpdateFont, bool random, int maxCols );
            
            int numCols();
            int numRows();
            
            void setText(string log);
            
            gl::Texture getTexture();
            gl::Texture getFadeTexture();
            
            Rectf getBounds();
            Vec2i getSize();
            
        private:
            
            FontManager myFontManager;
            
            int myFontSize;
            Vec2i mySize;
            Vec2i myMatrixSize;
            
            int myCurrentCol;
            int myCurrentRow;
            
            gl::Fbo myFboTmp;
            gl::Fbo myFadeFboTmp;
            
            Slide2DEffect myFbo;
            Slide2DEffect myFadeFbo;
            
            float mySlideY;
            
        };
        
    }
    
}