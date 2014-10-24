#pragma once

#include "AppBase.h"
#include "EventDispatcher.h"

using namespace sgmnt;
using namespace sgmnt::app;

namespace fl{ namespace display{
    
    class FlDisplayObject : public sgmnt::events::EventDispatcher{
        
        public:
        
            FlDisplayObject(){
                EventDispatcher();
            };
        
            FlDisplayObject( AppBase * app ){
                EventDispatcher();
                setup( app );
            };
            
            virtual void setup( AppBase * app ){
                mAppBase = app;
            };
        
            virtual float getX(){
                return mBounds.x1;
            }
            virtual void setX( float x ){
                float diff = mBounds.x1 - x;
                mBounds.x1 += diff;
                mBounds.x2 += diff;
            };
        
            virtual float getY(){
                return mBounds.y1;
            }
            virtual void setY( float y ){
                float diff = mBounds.y1 - y;
                mBounds.y1 += diff;
                mBounds.y2 += diff;
            };
        
            virtual float getWidth(){
                return mBounds.getWidth();
            }
            virtual void setWidth( float w ){
                mBounds.x2 = mBounds.x1 + w;
            };
            
            virtual float getHeight(){
                return mBounds.getHeight();
            }
            virtual void setHeight( float h ){
                mBounds.y2 = mBounds.y1 + h;
            };
        
            virtual void update(){
                // Need to override.
            };
        
            virtual void draw(){
                // Need to override.
            };
        
        protected :
        
            AppBase*    mAppBase;
            Rectf       mBounds;
            
    };
    
    /**
     * FBO based DisplayObject.
     */
    class FlFboDisplayObject : public FlDisplayObject{
        
        public :
        
            FlFboDisplayObject(){
                FlDisplayObject();
            }
        
            FlFboDisplayObject( AppBase * app, Vec2i fboSize ){
                FlDisplayObject();
                setup( app, fboSize );
            }
        
            virtual void setup( AppBase * app, Vec2i fboSize ){
                FlDisplayObject::setup(app);
                gl::Fbo::Format format;
                mFbo        = gl::Fbo( fboSize.x, fboSize.y, format );
                mOutputFbo  = gl::Fbo( fboSize.x, fboSize.y, format );
                mFboSize    = fboSize;
                mFboAspect  = sgmnt::utils::getAspectRatio( fboSize );
                mBounds.set( 0, 0, fboSize.x, fboSize.y );
            }
        
            virtual void update(){
                Area viewport = gl::getViewport();
                gl::setViewport( (Area)mBounds );
                gl::pushMatrices();
                    gl::setMatricesWindow( mFboSize, false );
                    mFbo.bindFramebuffer();
                        mUpdate();
                    mFbo.unbindFramebuffer();
                    mUpdateAfter();
                    mOutputFbo.bindFramebuffer();
                        mDrawToOutput();
                    mOutputFbo.unbindFramebuffer();
                gl::popMatrices();
                gl::setViewport(viewport);
            }
        
            virtual void draw(){
                gl::draw( getTexture(), mBounds );
            }
            
            virtual void draw( Rectf bounds ){
                gl::draw( getTexture(), bounds );
            }
        
            virtual gl::Texture getTexture(){
                return mOutputFbo.getTexture();
            }
        
        protected :
        
            gl::Fbo mFbo;
            gl::Fbo mOutputFbo;
            Vec2i   mFboSize;
            Vec2i   mFboAspect;
        
            virtual void mUpdate(){}
        
            virtual void mUpdateAfter(){}
        
            virtual void mDrawToOutput(){
                gl::clear();
                gl::draw( mFbo.getTexture() );
            }
        
    };
    
}};