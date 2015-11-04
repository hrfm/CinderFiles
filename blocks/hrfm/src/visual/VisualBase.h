#pragma once

#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"

#include "AppBase.h"
#include "FlDisplayObject.h"
#include "Utils.h"

using namespace hrfm;
using namespace hrfm::app;
using namespace hrfm::utils;

using namespace fl::display;

namespace hrfm { namespace visual{
    
    class VisualBase : public FlFboDisplayObject{
        
        public:
        
            VisualBase(){
                FlFboDisplayObject();
            };
            
            VisualBase( AppBase * app, ivec2 resolution ){
                FlFboDisplayObject( app, resolution );
            }
        
            virtual void setup( AppBase * app, ivec2 resolution ){
                FlFboDisplayObject::setup( app, resolution );
            };
            
            virtual void update(){
                FlFboDisplayObject::update();
            };
        
            virtual void draw(){};
        
            virtual ci::gl::Texture output(){
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
    
}};