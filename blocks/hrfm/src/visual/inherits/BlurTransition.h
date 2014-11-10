#pragma once

#include "cinder/gl/Texture.h"
#include "VisualBase.h"
#include "OutputEffect.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace hrfm;
using namespace hrfm::visual;

namespace hrfm{ namespace visual{
    
    class BlurTransition : public VisualTransitionBase{
        
        public :
        
            BlurTransition(){
                VisualTransitionBase();
            };
        
            void init( Vec2i size, float transitionTime ){
                VisualTransitionBase::init( size, transitionTime );
                mBlurEffectV.setup( size );
                mBlurEffectH.setup( size );
            };
            
            void transition( float progress ){
                
                gl::Texture inputTex;
                
                inputTex = mCurrent->output();
                mBlurEffectV.setUniform(Vec2f(0.00,0.01*mProgress), 1.0);
                mBlurEffectH.setUniform(Vec2f(0.01*mProgress,0.00), 1.0);
                mBlurEffectH.affect( ( mBlurEffectV.affect( inputTex ) ) );
                
                mOutputFbo.bindFramebuffer();
                    gl::clear();
                    gl::enableAdditiveBlending();
                    gl::pushMatrices();
                    gl::setMatricesWindow( Vec2i( mResolution.x, mResolution.y ), false );
                        gl::color( ColorA( 1.0f, 1.0f, 1.0f, 1.0f - mProgress ) );
                        gl::draw( mBlurEffectH.getTexture() );
                    gl::popMatrices();
                mOutputFbo.unbindFramebuffer();
                
                inputTex = mNext->output();
                mBlurEffectV.setUniform(Vec2f(0.00,0.01*(1.0f-mProgress)), 1.0);
                mBlurEffectH.setUniform(Vec2f(0.01*(1.0f-mProgress),0.00), 1.0);
                mBlurEffectH.affect( ( mBlurEffectV.affect( inputTex ) ) );
                
                mOutputFbo.bindFramebuffer();
                    gl::enableAdditiveBlending();
                    gl::pushMatrices();
                    gl::setMatricesWindow( Vec2i( mResolution.x, mResolution.y ), false );
                        gl::color( ColorA( 1.0f, 1.0f, 1.0f, mProgress ) );
                        gl::draw( mBlurEffectH.getTexture(), getWindowBounds() );
                    gl::popMatrices();
                mOutputFbo.unbindFramebuffer();
                
            };
            
        private :
        
            Blur2DEffect mBlurEffectV;
            Blur2DEffect mBlurEffectH;
            Base2DEffect mLastEffect;
        
    };
    
}}