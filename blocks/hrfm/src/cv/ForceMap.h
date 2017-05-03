#pragma once

#include <math.h>

using namespace std;
using namespace ci;

namespace hrfm{ namespace cv{
    
    class ForceNode{
        
        public :
        
            ForceNode( ivec2 pos, float decline = 0.4 ){
                mPosition    = pos;
                mForce       = vec2( 0.0, 0.0 );
                mAddedForce  = vec2( 0.0, 0.0 );
                mPropagation = vec2( 0.0, 0.0 );
                mDecline     = decline;
            }
        
            ivec2 getPosition(){
                return mPosition;
            }
            
            void update(){
                mForce *= mDecline;
                mForce += mAddedForce;
                mPropagation = mAddedForce;
                mAddedForce.x = 0.0;
                mAddedForce.y = 0.0;
            };
            
            void addForce( vec2 force ){
                mAddedForce += force;
            }
        
            vec2 getForce(){
                return mForce;
            }
        
            vec2 getPropagation(){
                return mPropagation;
            }
        
        protected :
        
            ivec2 mPosition;
        
            vec2 mForce;
            vec2 mAddedForce;
            vec2 mPropagation;
        
            float mDecline;
            
    };
    
    class ForceMap{
        
        public :
        
            ForceMap(){}
            
            void setup( int cols, int rows, float decline = 0.4 ){
                mCols   = cols;
                mRows   = rows;
                mSize   = ivec2( mCols, mRows );
                for( int row = 0; row < mRows; row++ ){
                    for( int col = 0; col < mCols; col++ ){
                        mForces.push_back( new ForceNode( ivec2(col,row), decline ) );
                    }
                }
                mSurface = SurfaceT<uint8_t>( mCols, mRows, true );
            }
            
            ivec2 getSize(){
                return mSize;
            }
            
            void addForce( vec2 position, vec2 force ){
                int col = round( position.x * (mCols-1) );
                int row = round( position.y * (mRows-1) );
                int idx = row * mCols + col;
                if( idx < 0 || mForces.size() <= idx ){
                    return;
                }
                mForces[idx]->addForce(force);
            }
        
            vec2 getForce( vec2 position ){
                int col = round( position.x * (mCols-1) );
                int row = round( position.y * (mRows-1) );
                return getForceAt( col, row );
            }
            
            vec2 getForceAt( int x, int y ){
                int idx = y * mCols + x;
                if( idx < 0 || mForces.size() <= idx ){
                    return vec2(0.0,0.0);
                }
                return mForces[idx]->getForce();
            }
        
            vec2 getForceAtf( float x, float y ){
                return getForceAt( floor( x * mCols ), floor( y * mRows ) );
            }
            
            void update(){
                
                mMaxForce.x = 0.0;
                mMaxForce.y = 0.0;
                mMaxForcePoint.x = 0.0;
                mMaxForcePoint.y = 0.0;
                
                vector<ForceNode*>::iterator it;
                const std::vector<ForceNode*>::const_iterator & end = mForces.end();
                
                // 力を伝播させる.
                //*
                for (it = mForces.begin(); it != end; ++it) {
                    
                    ivec2 pos   = (*it)->getPosition();
                    vec2 force = (*it)->getPropagation();
                    
                    int   idx   = 0;
                    int   col   = pos.x;
                    int   row   = pos.y;
                    
                    if( ci::length(force) < 0.01 ){
                        continue;
                    }
                    
                    float radian = atan2( force.y, force.x );
                    vec2 angled = vec2( cos(radian), sin(radian) );
                    vec2 angledAbs = vec2( abs( angled.x ), abs( angled.y ) );
                    
                    if( 0 < col && angled.x < 0 ){
                        idx = row * mCols + col - 1;
                        mForces[idx]->addForce( force * angledAbs * 0.4f );
                    }
                    
                    if( col < mCols - 1 && 0 < angled.x ){
                        idx = row * mCols + col + 1;
                        mForces[idx]->addForce( force * angledAbs * 0.4f );
                    }
                    
                    if( 0 < row && angled.y < 0 ){
                        idx = (row-1) * mCols + col;
                        mForces[idx]->addForce( force * angledAbs * 0.4f );
                    }
                    
                    if( row < mRows - 1 && 0 < angled.y ){
                        idx = (row+1) * mCols + col;
                        mForces[idx]->addForce( force * angledAbs * 0.4f );
                    }
                    
                }
                //*/
                
                //*/
                
                // 最大の力と平均を調べる.
                for (it = mForces.begin(); it != end; ++it) {
                    (*it)->update();
                    ivec2 pos   = (*it)->getPosition();
                    vec2 force = (*it)->getForce();
                    float forceLength = ci::length(force);
                    if( 0.1 < forceLength ){
                        //mAverageForce = ( mAverageForce + force ) / 2.0;
                        if( ci::length(mMaxForce) < forceLength ){
                            mMaxForce = force;
                            mMaxForcePoint.x = (float)pos.x / (float)(mCols-1);
                            mMaxForcePoint.y = (float)pos.y / (float)(mRows-1);
                        }
                    }
                }
                
                mAverageForce = ( mAverageForce + mMaxForce ) / 2.0f;
                
                //*
                for (it = mForces.begin(); it != end; ++it) {
                    
                    ivec2 pos   = (*it)->getPosition();
                    vec2 force = (*it)->getForce();
                    uint  r=0, g=0, b=0, a=0;
                    
                    if( force.x < 0.0 ){
                        r = floor( -force.x / 10.0f * 255.0 );
                        if( 255 < r ) r = 255;
                    }else{
                        g = floor(  force.x / 10.0f * 255.0 );
                        if( 255 < g ) g = 255;
                    }
                    
                    if( force.y < 0.0 ){
                        b = floor( -force.y / 10.0f * 255.0 );
                        if( 255 < b ) b = 255;
                    }else{
                        a = floor(  force.y / 10.0f * 255.0 );
                        if( 255 < a ) a = 255;
                    }
                    
                    mSurface.setPixel( pos, ColorA8u( r, g, b, a ) );
                    
                }
                
                mTexture = ci::gl::Texture2d::create( mSurface );
                //*/
                
            }
        
            ci::gl::Texture2dRef getForceTexture(){
                return mTexture;
            }
        
            vec2 getAverageForce(){
                return mAverageForce;
            }
        
            vec2 getMaxForce(){
                return mMaxForce;
            }
        
            vec2 getMaxForcePosition(){
                return mMaxForcePoint;
            }
            
            void draw( vec2 size, float lineWidth = 2.0 ){
                vec2 force, from;
                ci::gl::disable(GL_TEXTURE_2D);
                {
                    ci::gl::begin( GL_LINES );
                    {
                        ci::gl::lineWidth( lineWidth );
                        for( int x=0; x < mCols; x++ ){
                            for( int y=0; y < mRows; y++ ){
                                force  = getForceAt( x, y );
                                from.x = (float)x / (float)(mCols-1) * size.x;
                                from.y = (float)y / (float)(mRows-1) * size.y;
                                ci::gl::vertex( from );
                                ci::gl::vertex( from + force );
                            }
                        }
                    }
                    ci::gl::end();
                }
                ci::gl::enable( GL_TEXTURE_2D );
            }
            
        protected :
        
            int     mCols;
            int     mRows;
            ivec2   mSize;
        
            Surface mSurface;
            ci::gl::Texture2dRef mTexture;
        
            vector<ForceNode*> mForces;
        
            vec2 mAverageForce;
            vec2 mMaxForce;
            vec2 mMaxForcePoint;
            
            ci::gl::FboRef motionFbo;
        
    };
    
}}