#pragma once

#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"

#include "AppBase.h"
#include "FlDisplayObject.h"
#include "Utils.h"

using namespace sgmnt;
using namespace sgmnt::app;
using namespace sgmnt::utils;

using namespace fl::display;

namespace sgmnt { namespace visual{
    
    class VisualBase : public FlFboDisplayObject{
        
        public:
        
            VisualBase(){
                FlFboDisplayObject();
            };
            
            VisualBase( AppBase * app, Vec2i resolution ){
                FlFboDisplayObject( app, resolution );
            }
        
            virtual void setup( AppBase * app, Vec2i resolution ){
                FlFboDisplayObject::setup( app, resolution );
            };
            
            virtual void update(){
                FlFboDisplayObject::update();
            };
        
            virtual void draw(){};
        
            virtual gl::Texture output(){
                return getTexture();
            };
            
            // ============================================================================
            // === TRANSITIONS ===
        
            // === IN.
        
            virtual void prepareTransitionIn(){
                mTransitionInCompleted = false;
            }
            
            // 表示アニメーション処理. transition の経過時間が渡されるのでそれに応じた表示を行う.
            // このクラスは無条件で true を返しますが, 継承先では表示終了が完了したら true を返しましょう.
            virtual bool transitionIn( float time ){
                mTransitionInCompleted = true;
                return mTransitionInCompleted;
            }
        
            virtual bool isTransitionInCompleted(){
                return mTransitionInCompleted;
            }
        
            // === OUT.
        
            virtual void prepareTransitionOut(){
                mTransitionOutCompleted = false;
            }
            
            // 表示終了アニメーション処理. transition の経過時間が渡されるのでそれに応じた表示を行う.
            // このクラスは無条件で true を返しますが, 継承先では表示終了が完了したら true を返しましょう.
            virtual bool transitionOut( float time ){
                mTransitionOutCompleted = true;
                return mTransitionOutCompleted;
            }
        
            virtual bool isTransitionOutCompleted(){
                return mTransitionOutCompleted;
            }
            
        protected :
        
            virtual void mUpdate(){
                
            }
        
            bool    mTransitionInCompleted;
            bool    mTransitionOutCompleted;
        
    };
    
    class StackedVisualBase : public VisualBase{
        
        public:
            
            StackedVisualBase( AppBase * app, Vec2i resolution, vector<VisualBase*> visuals ){
                VisualBase();
                setup( app, resolution, visuals );
            };
            
            virtual void setup( AppBase * app, Vec2i resolution, vector<VisualBase*> visuals ){
                VisualBase::setup( app, resolution );
                mVisuals = visuals;
            };
        
            virtual void addVisual( VisualBase * visual ){
                mVisuals.push_back( visual );
            }
        
            virtual void update(){
                vector<VisualBase*>::iterator it;
                const std::vector<VisualBase*>::const_iterator & end = mVisuals.end();
                for (it = mVisuals.begin(); it != end; ++it) {
                    (*it)->update();
                }
                VisualBase::update();
            };
            
        protected :
        
            void mUpdate(){
                vector<VisualBase*>::iterator it;
                const std::vector<VisualBase*>::const_iterator & end = mVisuals.end();
                gl::enableAdditiveBlending();
                gl::clear();
                gl::color( ColorA( 1.0, 1.0, 1.0, 1.0 ) );
                for (it = mVisuals.begin(); it != end; ++it) {
                    gl::draw( (*it)->output(), mFboSize );
                }
                gl::disableAlphaBlending();
            };
            
            vector<VisualBase*> mVisuals;
        
    };
    
    class VisualTransitionBase{
        
        public:
        
            VisualTransitionBase(){
                mProgress  = 1.0;
                mStartTime = -1.0;
            };
        
            virtual void init( Vec2i size, float transitionTime ){
                gl::Fbo::Format format;
                mResolution = size;
                mAspect     = getAspectRatio( size );
                mOutputFbo  = gl::Fbo( mResolution.x, mResolution.y, format );
                mTransTime  = transitionTime;
            };
            
            virtual void setup( VisualBase * current, VisualBase * next ){
                setup( current, next, mTransTime );
            };
            
            virtual void setup( VisualBase * current, VisualBase * next, float transitionTime ){
                
                mProgress   = 0.0;
                mInterval   = transitionTime;
                mStartTime  = ci::app::getElapsedSeconds();
                mCurrent    = current;
                mNext       = next;
                
                // Transition の準備.
                mCurrent->prepareTransitionOut();
                mNext->prepareTransitionIn();
                
                mOutputFbo.bindFramebuffer();
                    gl::clear();
                mOutputFbo.unbindFramebuffer();
                
            }
        
            virtual void setTransitionTime( float transitionTime ){
                mInterval = transitionTime;
            }
            
            float getTransitionTime(){
                return mInterval;
            }
        
            virtual void update(){
                
                if( isCompleted() ){
                    // Return immediately if transitions completed.
                    return;
                }
                
                double now         = ci::app::getElapsedSeconds();
                double elapsedTime = now - mStartTime;
                
                if( 1.0 <= mProgress ){
                    // Update next visual for transition in.
                    nextVisualTransitionIn(elapsedTime);
                    mNext->update();
                    mNext->draw();
                }else if( mCurrent->isTransitionOutCompleted() ){
                    mProgress = ( elapsedTime / mInterval );
                    if( 1.0 < mProgress ){
                        mProgress = 1.0;
                        mStartTime = now;
                    }
                    mNext->update();
                    mNext->draw();
                    transition( mProgress );
                }else{
                    currentVisualTransitionOut( elapsedTime );
                    mCurrent->update();
                    mCurrent->draw();
                }
                
            };
        
            virtual void nextVisualTransitionIn( float elapsedTime ){
                mNext->transitionIn( elapsedTime );
            }
        
            virtual void currentVisualTransitionOut( float elapsedTime ){
                if( mCurrent->transitionOut( elapsedTime ) ){
                    mStartTime = ci::app::getElapsedSeconds();
                }
            }
        
            virtual void transition( float progress ){
                // Need to override.
            }
            
            virtual bool isCompleted(){
                if( 0.0 <= mStartTime ){
                    try{
                        return mNext->isTransitionInCompleted();
                    }catch(...){
                        return true;
                    }
                }else{
                    // Always return true before setup completed.
                    return true;
                }
            };
            
            virtual gl::Texture output(){
                if( 1.0 <= mProgress ){
                    return mNext->output();
                }else if( mCurrent->isTransitionOutCompleted() ){
                    return mOutputFbo.getTexture();
                }else{
                    return mCurrent->output();
                }
            };
            
        protected:
        
            gl::Fbo     mOutputFbo;
            Vec2i       mResolution;
            Vec2i       mAspect;
            float       mTransTime;
        
            double      mStartTime;
            double      mInterval;
            double      mProgress;
            VisualBase  *mCurrent;
            VisualBase  *mNext;
        
    };
    
}};