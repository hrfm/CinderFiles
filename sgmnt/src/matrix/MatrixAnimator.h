#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "MatrixLine.h"
#include "MatrixWave.h"

using namespace ci;
using namespace ci::gl;
using namespace ci::app;
using namespace std;

namespace sgmnt {
    
    namespace matrix{
        
        class BaseMatrixAnimator{
            public:
                virtual void setup( Vec2i size, int cellSize ){
                    // init Property.
                    setup( size, cellSize, cellSize );
                };
                virtual void setup( Vec2i size, int colSize, int rowSize ){
                    // init Property.
                    mySize = size;
                    myCols = ceil( (float)size.x / (float)colSize );
                    myRows = ceil( (float)size.y / (float)rowSize );
                    myColSize = (float)mySize.x / (float)myCols;
                    myRowSize = (float)mySize.y / (float)myRows;
                    // init Fbo.
                    gl::Fbo::Format format;
                    myFbo = gl::Fbo( size.x, size.y, format );
                };
                virtual void update( Channel32f * mChannel ){};
                gl::Texture getTexture(){
                    return myFbo.getTexture();
                }
                void bindTexture( int index ){
                    myFbo.bindTexture(index);
                }
                void unbindTexture(){
                    myFbo.unbindTexture();
                }
            protected:
                Vec2i mySize;
                int myCols;
                int myRows;
                float myColSize;
                float myRowSize;
                gl::Fbo myFbo;
        };
        
        class MatrixAnimator : public BaseMatrixAnimator {
            public:
                MatrixAnimator(){
                    BaseMatrixAnimator();
                }
                void setup( Vec2i size, int cellSize );
                void update( Channel32f * mChannel );
            private:
                int myNumLines;
                vector<MatrixLine*> myMatrixLines;
        };
        
        class WaveMatrixAnimator : public BaseMatrixAnimator{
            public:
                WaveMatrixAnimator(){
                    BaseMatrixAnimator();
                }
                void setup( Vec2i size, int cellSize );
                void setup( Vec2i size, int colSize, int rowSize );
                void update( Channel32f * mChannel );
                void addWaveAt( int col, float pow = 0.08f, float decline = 0.82, float t = 30 ){
                    myWaveNodes[col]->addWave(pow, decline, t);
                }
            private:
                vector<WaveNode*> myWaveNodes;
                vector<float> myList;
        };
        
    }
    
}