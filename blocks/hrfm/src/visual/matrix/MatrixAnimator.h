#pragma once

#include "cinder/app/App.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "MatrixLine.h"
#include "MatrixWave.h"

using namespace ci;
using namespace ci::gl;
using namespace std;

namespace hrfm { namespace matrix{
    
    class BaseMatrixAnimator{
    public:
        virtual void setup( ivec2 size, int cellSize ){
            // init Property.
            setup( size, cellSize, cellSize );
        };
        virtual void setup( ivec2 size, int colSize, int rowSize ){
            // init Property.
            mySize = size;
            myCols = ceil( (float)size.x / (float)colSize );
            myRows = ceil( (float)size.y / (float)rowSize );
            myColSize = (float)mySize.x / (float)myCols;
            myRowSize = (float)mySize.y / (float)myRows;
            // init Fbo.
            ci::gl::Fbo::Format format;
            myFbo = ci::gl::Fbo::create( size.x, size.y, format );
        };
        virtual void update( Channel32f * mChannel ){};
        ci::gl::TextureRef getTexture(){
            return myFbo->getColorTexture();
        }
        void bindTexture( int index ){
            myFbo->bindTexture(index);
        }
        void unbindTexture(){
            myFbo->unbindTexture();
        }
    protected:
        ivec2 mySize;
        int myCols;
        int myRows;
        float myColSize;
        float myRowSize;
        ci::gl::FboRef myFbo;
    };
    
    class MatrixAnimator : public BaseMatrixAnimator {
    public:
        MatrixAnimator(){
            BaseMatrixAnimator();
        }
        void setup( ivec2 size, int cellSize );
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
        void setup( ivec2 size, int cellSize );
        void setup( ivec2 size, int colSize, int rowSize );
        void update( Channel32f * mChannel );
        void addWaveAt( int col, float pow = 0.08f, float decline = 0.82, float t = 30 ){
            myWaveNodes[col]->addWave(pow, decline, t);
        }
    private:
        vector<WaveNode*> myWaveNodes;
        vector<float> myList;
    };
    
}}