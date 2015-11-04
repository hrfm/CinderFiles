#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"

using namespace ci;
using namespace ci::gl;
using namespace std;

namespace hrfm {
    
    namespace matrix{
        
        // Matrix 上に表示されるラインを再現するためのクラス.
        // タテもしくはヨコ方向に光るラインを定義する.
        class MatrixLine{
            
            public :
            
            MatrixLine( int cols, int rows ){
                
                myCols = cols;
                myRows = rows;
                
                myCol = (float)randInt( 0, cols-1 );
                myRow = (float)randInt( 0, rows-1 );
                
                // 移動方向と基本速度を定義.
                myDirection = 2;
                myBaseSpeed = randFloat(0.2,0.8);
                
                // 下方向へ移動する速度を設定.
                mySpeedX = 0.0;
                mySpeedY = myBaseSpeed;
                
                mySpeedMin = 0.1;
                mySpeedMax = 0.3;
                
            };
            
            void update( Channel32f * mChannel, float mChannelWidth, float mChannelHeight, float w, float h ){
                
                // 速度の通りセル移動.
                myCol += mySpeedX;
                myRow += mySpeedY;
                
                // ユーザインタラクションの定義.
                // 背景差分法で動きがあった場所の動きを変化させる.
                mivec2.x = floor( ( myCol / myCols ) * mChannelWidth );
                mivec2.y = floor( ( myRow / myRows ) * mChannelHeight );
                
                if( 0.0 < mChannel->getValue( mivec2 ) ){
                    float rand = randFloat();
                    float speed = randFloat(1.0,2.0);
                    if( rand < 0.25 ){
                        myDirection = 0;
                        mySpeedX = 0.0;
                        mySpeedY = -speed;
                    }else if( rand < 0.50 ){
                        myDirection = 1;
                        mySpeedX = speed;
                        mySpeedY = 0.0;
                    }else if( rand < 0.75 ){
                        myDirection = 3;
                        mySpeedX = -speed;
                        mySpeedY = 0.0;
                    }else{
                        myDirection = 2;
                        mySpeedX = 0.0;
                        mySpeedY = speed;
                    }
                }
                
                // 移動方向に応じてその速度を myBaseSpeed に近似させていく.
                
                float f = 0.36;
                float speedDiff;
                
                if( myDirection == 0 ){
                    // 上方向の処理.
                    mySpeedY += ( -myBaseSpeed - mySpeedY ) * f;
                    speedDiff = abs( -myBaseSpeed - mySpeedY );
                }else if( myDirection == 1 ){
                    // 右方向の処理.
                    mySpeedX += ( myBaseSpeed - mySpeedX ) * f;
                    speedDiff = abs( myBaseSpeed - mySpeedX );
                }else if( myDirection == 2 ){
                    // 下方向の処理.
                    mySpeedY += ( myBaseSpeed - mySpeedY ) * f;
                    speedDiff = abs( myBaseSpeed - mySpeedY );
                }else if( myDirection == 3 ){
                    // 左方向の処理.
                    mySpeedX += ( -myBaseSpeed - mySpeedX ) * f;
                    speedDiff = abs( -myBaseSpeed - mySpeedX );
                }
                
                if( myRows <= myRow || myRow < 0 || myCols <= myCol || myCol < 0 ){
                    // 最下部まで達した場合上に戻し,ヨコ位置をランダムに決定し下に落とす.
                    myRow = 0.0;
                    myCol = (float)randInt( 0, myCols-1 );
                    // 現在の挙動をリセットする.
                    myDirection = 2;
                    myBaseSpeed = randFloat(mySpeedMin,mySpeedMax);
                    mySpeedX    = 0.0;
                    mySpeedY    = myBaseSpeed;
                }
                
                if( 0.001f <= speedDiff ){
                    ci::gl::color(1.0,0.0,0.0);
                }else{
                    if( 0.0f < speedDiff ){
                        myDirection = 2;
                        myBaseSpeed = randFloat(mySpeedMin,mySpeedMax);
                        mySpeedX    = 0.0;
                        mySpeedY    = myBaseSpeed;
                    }
                    ci::gl::color(0.8,0.8,0.8);
                }
                
                mRectf.set( floor(myCol)*w, floor(myRow)*h, floor(myCol+1.0f)*w, floor(myRow+1.0f)*h );
                ci::gl::drawSolidRect( mRectf );
                
            }
            
        private:
            
            // アニメーションすべきセルの総数.
            int myCols;
            int myRows;
            
            // 現在アニメーションしているセルの位置.
            float myCol;
            float myRow;
            
            // 次のセルへ移動する速度.
            int myDirection; // 移動方向 0 上 1 右 2 下 3 左
            float myBaseSpeed;
            float mySpeedMin;
            float mySpeedMax;
            float mySpeedX;
            float mySpeedY;
            
            // 内部的に使う geom
            ivec2 mivec2;
            Rectf mRectf;
            
        };
        
    }
    
}