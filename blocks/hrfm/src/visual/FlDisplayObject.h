#pragma once

#include "AppBase.h"
#include "EventDispatcher.h"

using namespace ci;
using namespace hrfm::app;
using namespace hrfm::display;
using namespace hrfm::events;

namespace fl{ namespace display{
    
    class FlDisplayObject : public IDrawable, public EventDispatcher{
        
    public:
        
        FlDisplayObject(){
            IDrawable();
            EventDispatcher();
        };
        
        FlDisplayObject( AppBase * app ){
            FlDisplayObject();
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
            this->x = x;
        };
        
        virtual float getY(){
            return mBounds.y1;
        }
        virtual void setY( float y ){
            float diff = mBounds.y1 - y;
            mBounds.y1 += diff;
            mBounds.y2 += diff;
            this->y = y;
        };
        
        virtual float getWidth(){
            return mBounds.getWidth();
        }
        virtual void setWidth( float w ){
            mBounds.x2 = mBounds.x1 + w;
            this->width = w;
        };
        
        virtual float getHeight(){
            return mBounds.getHeight();
        }
        virtual void setHeight( float h ){
            mBounds.y2 = mBounds.y1 + h;
            this->height = h;
        };
        
        virtual void _update(){
            // Need to override.
        };
        
        virtual void _draw(){
            // Need to override.
        };
        
    protected:
        
        AppBase*    mAppBase;
        Rectf       mBounds;
        
    };
    
    /**
     * FBO based DisplayObject.
     */
    class FlFboDisplayObject : public FlDisplayObject{
        
    public:
        
        FlFboDisplayObject(){
            FlDisplayObject();
        }
        
        FlFboDisplayObject( AppBase * app, Vec2i fboSize ){
            FlDisplayObject();
            setup( app, fboSize );
        }
        
        virtual void setup( AppBase * app, Vec2i fboSize ){
            FlDisplayObject::setup(app);
            ci::gl::Fbo::Format format;
            mFbo        = ci::gl::Fbo( fboSize.x, fboSize.y, format );
            mOutputFbo  = ci::gl::Fbo( fboSize.x, fboSize.y, format );
            mFboSize    = fboSize;
            mFboAspect  = hrfm::utils::getAspectRatio( fboSize );
            mBounds.set( 0, 0, fboSize.x, fboSize.y );
        }
        
        virtual ci::gl::Texture getTexture(){
            return mOutputFbo.getTexture();
        }
        
        virtual void update(){
            Area viewport = ci::gl::getViewport();
            ci::gl::setViewport( (Area)mBounds );
            ci::gl::pushMatrices();
            ci::gl::setMatricesWindow( mFboSize, false );
            mFbo.bindFramebuffer();
            mUpdate();
            mFbo.unbindFramebuffer();
            mUpdateAfter();
            mOutputFbo.bindFramebuffer();
            mDrawToOutput();
            mOutputFbo.unbindFramebuffer();
            ci::gl::popMatrices();
            ci::gl::setViewport(viewport);
        }
        
        virtual void draw(){
            ci::gl::draw( getTexture(), mBounds );
        }
        
        virtual void draw( Rectf bounds ){
            ci::gl::draw( getTexture(), bounds );
        }
        
    protected:
        
        virtual void mUpdate(){}
        
        virtual void mUpdateAfter(){}
        
        virtual void mDrawToOutput(){
            ci::gl::clear();
            ci::gl::draw( mFbo.getTexture() );
        }
        
        ci::gl::Fbo mFbo;
        ci::gl::Fbo mOutputFbo;
        Vec2i   mFboSize;
        Vec2i   mFboAspect;
        
    };
    
}};