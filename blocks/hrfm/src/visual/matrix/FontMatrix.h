#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "FontManager.h"
#include "Filter.h"
#include "OutputEffect.h"

using namespace ci;
using namespace ci::gl;
using namespace std;
using namespace hrfm::utils;
using namespace hrfm::gl::filter;

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
            
            ci::gl::Texture getTexture();
            ci::gl::Texture getFadeTexture();
            
            Rectf getBounds();
            Vec2i getSize();
            
        private:
            
            FontManager myFontManager;
            
            int myFontSize;
            Vec2i mySize;
            Vec2i myMatrixSize;
            
            int myCurrentCol;
            int myCurrentRow;
            
            ci::gl::Fbo myFboTmp;
            ci::gl::Fbo myFadeFboTmp;
            
            ScrollFilter myFbo;
            ScrollFilter myFadeFbo;
            
            float mySlideY;
            
        };
        
    }
    
}