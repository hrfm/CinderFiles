#pragma once

#include "AppBase.h"

using namespace ci;

using namespace hrfm;
using namespace hrfm::app;
using namespace hrfm::utils;

namespace hrfm { namespace visual{
    
    class ParticleBase{
        
    public:
        
        ParticleBase(){
            setup( vec3( 0.0, 0.0, 0.0 ) );
        };
        
        ParticleBase( vec3 position ){
            setup( position );
        }
        
        virtual void setup( vec3 position ){
            mQuat.set( 0, position.x, position.y, position.z );
            mQuat.identity();
            mQuat.normalize();
            //mPol.set( randFloat(-1.0,1.0), randFloat(-1.0,1.0), randFloat(-1.0,1.0) );
            //mPol.normalize();
            mPol.set( 0.1, 1.0, 0.0 );
        };
        
        virtual void update( float scale = 1.0 ){
            
            float theta = getElapsedSeconds();
            float sinTheta = sin( theta / 2.0 );
            
            Quatf Q,R,quat;
            vec3 QPol;
            
            QPol = mPol * sinTheta;
            Q.set( cos( theta/2.0 ), QPol.x, QPol.y, QPol.z );
            R.set( cos( theta/2.0 ), -QPol.x, -QPol.y, -QPol.z );
            quat = R * mQuat * Q;
            
            /*
             QPol = quat.v;
             QPol.x += sin(pi/2.0);
             QPol.z += cos(pi/2.0);
             QPol *= sin( scale / 2.0 );
             Q.set( cos( scale/2.0 ), QPol.x, QPol.y, QPol.z );
             R.set( cos( scale/2.0 ), -QPol.x, -QPol.y, -QPol.z );
             quat = R * quat * Q;
             //*/
            
            mRecentPosition = mPosition;
            mPosition.set( quat.v.x, quat.v.y, quat.v.z );
            mPosition *= 0.5;
            
        };
        
        virtual vec3 getPosition(){
            return mPosition;
        }
        
        virtual vec3 getRecentPosition(){
            return mRecentPosition;
        }
        
        virtual void draw(){
            gl::drawLine( mRecentPosition, mPosition );
        };
        
    protected:
        
        Quatf   mQuat;
        vec3   mPol;
        vec3   mPosition;
        vec3   mRecentPosition;
        
    };
    
}};