#include "MatrixAnimator.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace hrfm{ namespace matrix{
    
    // ==== MatrixAnimator ============================================================
    
    void MatrixAnimator::setup( Vec2i size, int cellSize ){
        this->BaseMatrixAnimator::setup(size,cellSize);
        myNumLines = 512;
        for( int i = 0; i < myNumLines; i++ ){
            myMatrixLines.push_back( new MatrixLine( myCols, myRows ) );
        }
    }
    
    void MatrixAnimator::update( Channel32f * mChannel ){
        
        float mChannelWidth  = mChannel->getWidth();
        float mChannelHeight = mChannel->getHeight();
        
        myFbo.bindFramebuffer();
        gl::pushMatrices();
        gl::setMatricesWindow(getWindowSize(),false);
        // --- フェード処理
        gl::color(0.0f,0.0f,0.0f,0.02f);
        gl::drawSolidRect(getWindowBounds());
        // update and draw MatrixLine.
        vector<MatrixLine*>::iterator it = myMatrixLines.begin();  // イテレータのインスタンス化
        while( it != myMatrixLines.end() ){
            (*it)->update( mChannel, mChannelWidth, mChannelHeight, myColSize, myRowSize );
            ++it;
        }
        gl::popMatrices();
        myFbo.unbindFramebuffer();
        
    }
    
    // ==== WaveMatrixAnimator ============================================================
    
    void WaveMatrixAnimator::setup( Vec2i size, int cellSize ){
        setup( size, cellSize, cellSize );
    }
    
    void WaveMatrixAnimator::setup( Vec2i size, int colSize, int rowSize ){
        this->BaseMatrixAnimator::setup( size, colSize, rowSize );
        for( int i = 0; i < myCols; i++ ){
            myWaveNodes.push_back( new WaveNode() );
            myList.push_back(0);
        }
        // 常に発生する波を設定する.
        myWaveNodes[0]->addWave( 1.0, 1.0, 128 );
        //myWaveNodes[0]->addWave( 6.0, 1.0, 328 );
    }
    
    void WaveMatrixAnimator::update( Channel32f * mChannel ){
        
        float p;
        Vec2i mVec2i;
        int len = myWaveNodes.size() - 1;
        float mChannelWidth  = mChannel->getWidth();
        float mChannelHeight = mChannel->getHeight();
        float level = (float)mySize.y / 3.0f * 2.0;
        
        vector<WaveNode*>::iterator it = myWaveNodes.begin();  // イテレータのインスタンス化
        while( it != myWaveNodes.end() ){
            (*it)->prepare();
            it++;
        }
        
        myFbo.bindFramebuffer();
        gl::pushMatrices();
        gl::setMatricesWindow(getWindowSize(),false);
        
        // フェード処理
        gl::color(0.0f,0.0f,0.0f,0.08f);
        gl::drawSolidRect(getWindowBounds());
        
        for( int i = 0; i <= len; i++ ){
            
            if( i == 0 ){
                p = myWaveNodes[i]->update( myWaveNodes[i+1]->getLeft(), myWaveNodes[len-1]->getRight() );
            }else if( i == len ){
                p = myWaveNodes[i]->update( myWaveNodes[0]->getLeft(), myWaveNodes[i-1]->getRight() );
            }else{
                p = myWaveNodes[i]->update( myWaveNodes[i+1]->getLeft(), myWaveNodes[i-1]->getRight() );
            }
            
            myList[i] += ( - p - myList[i] ) * 0.1;
            
            float x1 = i * myColSize;
            float x2 = (i+1) * myColSize;
            float y1 = mySize.y;
            float y2 = mySize.y - level - myList[i]*10;
            y2 = floor( y2 / myRowSize ) * myRowSize;
            
            for( int y = y2; y < mySize.y; y++ ){
                mVec2i.x = floor( ( x1 / mySize.x ) * mChannelWidth );
                mVec2i.y = floor( ( (float)y / mySize.y ) * mChannelHeight );
                if( 0.0 < mChannel->getValue(mVec2i) ){
                    addWaveAt(i);
                    break;
                }
            }
            
            gl::color(0.5f,0.9f,1.0f);
            gl::drawSolidRect( Rectf( x1, y1, x2, y2 ) );
            
            float alpha = 0.05f;
            y2 += myRowSize;
            while( y2 < mySize.y ){
                gl::color(0.0f,0.0f,0.0f,alpha);
                gl::drawSolidRect( Rectf( x1, y2, x2, y2+myRowSize ) );
                y2 += myRowSize;
                alpha += 0.08f;
            }
            
        }
        
        gl::popMatrices();
        myFbo.unbindFramebuffer();
        
    }
    
}}