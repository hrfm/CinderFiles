#pragma once

#include "cinder/gl/Texture.h"
#include "VisualBase.h"
#include "OutputEffect.h"

#include "BlurFilter.h"

using namespace ci;
using namespace std;

using namespace hrfm;
using namespace hrfm::visual;
using namespace hrfm::gl::filter;

namespace hrfm{ namespace visual{
    
    class BlurTransition : public VisualTransitionBase{
        
        public :
        
            BlurTransition(){
                VisualTransitionBase();
            };
        
            void init( ivec2 size, float transitionTime ){
                VisualTransitionBase::init( size, transitionTime );
                mBlurEffectV.setup( size );
                mBlurEffectH.setup( size );
            };
            
            void transition( float progress ){
                
                ci::gl::Texture inputTex;
                
                inputTex = mCurrent->output();
                mBlurEffectV.setUniform(vec2(0.00,0.01*mProgress), 1.0);
                mBlurEffectH.setUniform(vec2(0.01*mProgress,0.00), 1.0);
                mBlurEffectH.affect( ( mBlurEffectV.affect( inputTex ) ) );
                
                mOutputFbo.bindFramebuffer();
                    ci::gl::clear();
                    ci::gl::enableAdditiveBlending();
                    ci::gl::pushMatrices();
                    ci::gl::setMatricesWindow( ivec2( mResolution.x, mResolution.y ), false );
                        ci::gl::color( ColorA( 1.0f, 1.0f, 1.0f, 1.0f - mProgress ) );
                        ci::gl::draw( mBlurEffectH.getTexture() );
                    ci::gl::popMatrices();
                mOutputFbo.unbindFramebuffer();
                
                inputTex = mNext->output();
                mBlurEffectV.setUniform(vec2(0.00,0.01*(1.0f-mProgress)), 1.0);
                mBlurEffectH.setUniform(vec2(0.01*(1.0f-mProgress),0.00), 1.0);
                mBlurEffectH.affect( ( mBlurEffectV.affect( inputTex ) ) );
                
                mOutputFbo.bindFramebuffer();
                    ci::gl::enableAdditiveBlending();
                    ci::gl::pushMatrices();
                    ci::gl::setMatricesWindow( ivec2( mResolution.x, mResolution.y ), false );
                        ci::gl::color( ColorA( 1.0f, 1.0f, 1.0f, mProgress ) );
                        ci::gl::draw( mBlurEffectH.getTexture(), getWindowBounds() );
                    ci::gl::popMatrices();
                mOutputFbo.unbindFramebuffer();
                
            };
            
        private :
        
            BlurFilter mBlurEffectV;
            BlurFilter mBlurEffectH;
            FilterBase mLastEffect;
        
    };
    
}}