#pragma once

#include "FilterBase.h"
#include "VisualBase.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace std;

using namespace hrfm;
using namespace hrfm::visual;

// ========================================================
//
// Emitt Objects Visual.
//
// ========================================================

namespace hrfm{ namespace visual{
    
    class BaseObject4Emitter{
        
        public :
        
            BaseObject4Emitter(){
                mAlive = true;
            };
            
            virtual void setup(){};
            
            virtual bool isAlive(){
                return mAlive;
            }
            
            virtual void update(){};
            
            virtual void draw(){};
            
        protected :
        
            Vec3f mPosition;
            bool  mAlive;
            
    };
    
    class EmitterVisual : public VisualBase{
        
        public :
        
            EmitterVisual(){
                VisualBase();
            }
            
            EmitterVisual( AppBase * app ){
                VisualBase();
                setup( app, Vec2i( 640, 480 ) );
            };
            
            EmitterVisual( AppBase * app, Vec2i resolution ){
                VisualBase();
                setup( app, resolution );
            };
            
            virtual void setup( AppBase * app, Vec2i resolution ){
                VisualBase::setup( app, resolution );
            };
            
            virtual void update(){
                
                _update();
                
                vector<BaseObject4Emitter*>::iterator it;
                const vector<BaseObject4Emitter*>::iterator & end = mEmitterObjects.end();
                
                for( it = mEmitterObjects.begin(); it != end; ++it ){
                    (*it)->update();
                }
                
            };
            
            void draw(){
                
                gl::enableDepthRead();
                gl::enableDepthWrite();
                
                gl::enableWireframe();
                
                vector<BaseObject4Emitter*>::iterator it;
                const vector<BaseObject4Emitter*>::iterator & end = mEmitterObjects.end();
                
                mOutputFbo.bindFramebuffer();
                gl::clear();
                gl::color( mAppBase->getColor() );
                gl::pushMatrices();
                gl::setMatricesWindow( getWindowSize(), false );
                for( it = mEmitterObjects.begin(); it != end; ++it ){
                    (*it)->draw();
                    if( (*it)->isAlive() == false ){
                        it = mEmitterObjects.erase( it );
                    }
                }
                gl::popMatrices();
                mOutputFbo.unbindFramebuffer();
                
                gl::disableDepthRead();
                gl::disableDepthWrite();
                
                gl::disableWireframe();
                
            };
        
            virtual void addObject( BaseObject4Emitter * obj ){
                mEmitterObjects.push_back( obj );
            }
            
        protected :
        
            virtual void _update(){
                // write update code if needed.
            }
        
            vector<BaseObject4Emitter*> mEmitterObjects;
        
        private :
        
        
    };
    
}}