#pragma once

#include <math.h>

using namespace std;

namespace sgmnt{ namespace live_effects{
    
    class ForceNode{
        
        public :
        
            ForceNode( Vec2i pos, float decline = 0.4 ){
                mPosition    = pos;
                mForce       = Vec2f( 0.0, 0.0 );
                mAddedForce  = Vec2f( 0.0, 0.0 );
                mPropagation = Vec2f( 0.0, 0.0 );
                mDecline     = decline;
            }
        
            Vec2i getPosition(){
                return mPosition;
            }
            
            void update(){
                mForce *= mDecline;
                mForce += mAddedForce;
                mPropagation = mAddedForce;
                mAddedForce.set( 0.0, 0.0 );
            };
        
            void addForce( Vec2f force ){
                mAddedForce += force;
            }
        
            Vec2f getForce(){
                return mForce;
            }
        
            Vec2f getPropagation(){
                return mPropagation;
            }
        
        protected :
        
            Vec2i mPosition;
        
            Vec2f mForce;
            Vec2f mAddedForce;
            Vec2f mPropagation;
        
            float mDecline;
            
    };
    
    class ForceMap{
        
        public :
        
            ForceMap(){}
            
            void setup( int cols, int rows, float decline = 0.4 ){
                mCols   = cols;
                mRows   = rows;
                mSize   = Vec2i( mCols, mRows );
                for( int row = 0; row < mRows; row++ ){
                    for( int col = 0; col < mCols; col++ ){
                        mForces.push_back( new ForceNode( Vec2i(col,row), decline ) );
                    }
                }
                mSurface = SurfaceT<uint8_t>( mCols, mRows, true );
            }
            
            Vec2i getSize(){
                return mSize;
            }
            
            void addForce( Vec2f position, Vec2f force ){
                int col = round( position.x * (mCols-1) );
                int row = round( position.y * (mRows-1) );
                int idx = row * mCols + col;
                if( idx < 0 || mForces.size() <= idx ){
                    return;
                }
                mForces[idx]->addForce(force);
            }
        
            Vec2f getForce( Vec2f position ){
                int col = round( position.x * (mCols-1) );
                int row = round( position.y * (mRows-1) );
                return getForceAt( col, row );
            }
            
            Vec2f getForceAt( int x, int y ){
                int idx = y * mCols + x;
                if( idx < 0 || mForces.size() <= idx ){
                    return Vec2f(0.0,0.0);
                }
                return mForces[idx]->getForce();
            }
            
            void update(){
                
                mMaxForce.set(0.0,0.0);
                mMaxForcePoint.set(0.0,0.0);
                
                vector<ForceNode*>::iterator it;
                const std::vector<ForceNode*>::const_iterator & end = mForces.end();
                
                // 力を伝播させる.
                //*
                for (it = mForces.begin(); it != end; ++it) {
                    
                    Vec2i pos   = (*it)->getPosition();
                    Vec2f force = (*it)->getPropagation();
                    
                    int   idx   = 0;
                    int   col   = pos.x;
                    int   row   = pos.y;
                    
                    if( force.length() < 0.01 ){
                        continue;
                    }
                    
                    float radian = atan2( force.y, force.x );
                    Vec2f angled = Vec2f( cos(radian), sin(radian) );
                    Vec2f angledAbs = Vec2f( abs( angled.x ), abs( angled.y ) );
                    
                    if( 0 < col && angled.x < 0 ){
                        idx = row * mCols + col - 1;
                        mForces[idx]->addForce( force * angledAbs * 0.4 );
                    }
                    
                    if( col < mCols - 1 && 0 < angled.x ){
                        idx = row * mCols + col + 1;
                        mForces[idx]->addForce( force * angledAbs * 0.4 );
                    }
                    
                    if( 0 < row && angled.y < 0 ){
                        idx = (row-1) * mCols + col;
                        mForces[idx]->addForce( force * angledAbs * 0.4 );
                    }
                    
                    if( row < mRows - 1 && 0 < angled.y ){
                        idx = (row+1) * mCols + col;
                        mForces[idx]->addForce( force * angledAbs * 0.4 );
                    }
                    
                }
                //*/
                
                //*/
                
                // 最大の力と平均を調べる.
                for (it = mForces.begin(); it != end; ++it) {
                    (*it)->update();
                    Vec2i pos   = (*it)->getPosition();
                    Vec2f force = (*it)->getForce();
                    float forceLength = force.length();
                    if( 0.1 < forceLength ){
                        //mAverageForce = ( mAverageForce + force ) / 2.0;
                        if( mMaxForce.length() < forceLength ){
                            mMaxForce = force;
                            mMaxForcePoint.set((float)pos.x / (float)(mCols-1),(float)pos.y / (float)(mRows-1));
                        }
                    }
                }
                
                mAverageForce = ( mAverageForce + mMaxForce ) / 2.0;
                
                //*
                for (it = mForces.begin(); it != end; ++it) {
                    
                    Vec2i pos   = (*it)->getPosition();
                    Vec2f force = (*it)->getForce();
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
                
                mTexture = gl::Texture( mSurface );
                //*/
                
            }
        
            gl::Texture getForceTexture(){
                return mTexture;
            }
        
            Vec2f getAverageForce(){
                return mAverageForce;
            }
        
            Vec2f getMaxForce(){
                return mMaxForce;
            }
        
            Vec2f getMaxForcePosition(){
                return mMaxForcePoint;
            }
            
            void draw( Vec2f size, float lineWidth = 2.0 ){
                Vec2f force, from;
                glDisable( GL_TEXTURE_2D );
                glBegin( GL_LINES );
                    gl::lineWidth( lineWidth );
                    for( int x=0; x < mCols; x++ ){
                        for( int y=0; y < mRows; y++ ){
                            force  = getForceAt( x, y );
                            from.x = (float)x / (float)(mCols-1) * size.x;
                            from.y = (float)y / (float)(mRows-1) * size.y;
                            gl::vertex( from );
                            gl::vertex( from + force );
                        }
                    }
                glEnd();
                glEnable( GL_TEXTURE_2D );
            }
            
        protected :
        
            int     mCols;
            int     mRows;
            Vec2i   mSize;
        
            Surface     mSurface;
            gl::Texture mTexture;
        
            vector<ForceNode*> mForces;
        
            Vec2f mAverageForce;
            Vec2f mMaxForce;
            Vec2f mMaxForcePoint;
            
            gl::Fbo motionFbo;
        
    };
    
}}