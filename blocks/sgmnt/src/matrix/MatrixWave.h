#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"

using namespace ci;
using namespace ci::gl;
using namespace std;

namespace sgmnt {
    
    namespace matrix{
        
        class DeclineWave{
            
            public :
            
                DeclineWave( float pow, float decline = 0.998, float t = 100.0 ){
                    mPower = pow;
                    m_t = 0;
                    mA = 2;
                    mT = t;
                    mDecline = decline;
                }
                
                float getPower(){
                    return mPower;
                }
                
                float getPosition(){
                    return mPower * mA * sin( (pi*2.0) * (m_t/mT) );
                }
                
                bool update(){
                    m_t += 1;
                    mPower *= mDecline;
                    if( abs(mPower) < 0.008 ){
                        mPower = 0;
                        return false;
                    }
                    return true;
                }
            
            private :
                float mPower;
                float m_t;
                float mA;
                float mT;
                float mDecline;
            
        };
        
        class WaveNode {
            
            public :
            
            WaveNode(){
                mCurrent = 0;
                mBeforeLeft  = 0;
                mBeforeRight = 0;
                mSumLeft  = 0;
                mSumRight = 0;
            }
            
            float getLeft(){
                return mBeforeLeft;
            }
            
            float getRight(){
                return mBeforeRight;
            }
            
            void addWave( float pow, float decline, float t ){
                mWaves.push_back( new DeclineWave( pow, decline, t ) );
            }
            
            void prepare(){
                mCurrent = 0;
                mBeforeLeft  = mSumLeft;
                mBeforeRight = mSumRight;
            }
            
            float update( float left, float right ){
                
                if( 0 < mWaves.size() ){
                    mCurrent = 0;
                    vector<DeclineWave*>::iterator it = mWaves.begin();  // イテレータのインスタンス化
                    while( it != mWaves.end() ){
                        if( !(*it)->update() ){
                            mWaves.erase(it);
                        }else{
                            mCurrent += (*it)->getPosition();
                            ++it;
                        }
                    }
                }
                
                mSumLeft  = mCurrent + left * mDecline;
                mSumRight = mCurrent + right * mDecline;
                
                mCurrent += left + right;
                
                return mCurrent;
                
            }
            
            private :
                float mDecline = 0.992;
                vector<DeclineWave*> mWaves;
                float mCurrent;
                float mBeforeLeft;
                float mBeforeRight;
                float mSumLeft;
                float mSumRight;
            
        };
        
    }
    
}