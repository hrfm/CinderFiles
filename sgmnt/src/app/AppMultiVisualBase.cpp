#include "AppMultiVisualBase.h"

// === NAMESPACE ===================================================================================================

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace sgmnt::visual;

// === AppMultiVisualBase ==========================================================================================

// 複数の VisualBase を扱うためのクラス.
// Transition を設定する事で間をつなぐ事も出来る.
namespace sgmnt{ namespace app{
    
    void AppMultiVisualBase::prepareSettings( Settings *settings ){
        AppBase::prepareSettings(settings);
    }
    
    void AppMultiVisualBase::setup(){
        AppBase::setup();
        mCurrentVisualIndex = 0;
        mUseTransition      = false;
    }
    
    void AppMultiVisualBase::setupTransition( VisualTransitionBase * transition ){
        mUseTransition      = true;
        mTransition         = transition;
    }
    
    void AppMultiVisualBase::update(){
        
        if( useCapture && !captureInput.captureAvailable() ){
            return;
        }
        
        AppBase::update();
        
        // --- Update Visual. ---
        
        // --- Update Transition. ---
        
        gl::color( ColorA( 1.0, 1.0, 1.0, 1.0 ) );
        
        if( mCurrentVisualIndex < 0 ){
            mCurrentVisualIndex = 0;
        }else if( mVisuals.size() <= mCurrentVisualIndex ){
            mCurrentVisualIndex = mVisuals.size() - 1;
        }
        
        if( mUseTransition && !mTransition->isCompleted() ){
            mTransition->update();
            if( mTransition->isCompleted() && 0 <= mNextVisualIndex ){
                changeVisual( mNextVisualIndex );
            }
            mTexture = mTransition->output();
        }else{
            mVisuals.at(mCurrentVisualIndex)->update();
            mVisuals.at(mCurrentVisualIndex)->draw();
            mTexture = mVisuals.at(mCurrentVisualIndex)->output();
        }
        
        // --- Update OpticalFlow. ---
        
        //captureInput.updateOpticalFlow(
        //getFaderValueAt(10) * getFaderValueAt(2) * 2.0
        //);
        
        // --- Update Particle. ---
        
        //mParticle.update( captureInput.getForceMap() );
        //activeEffect.update( captureInput.getForceMap() );
        
        // --- Update Visual. ---
        
    }
    
    void AppMultiVisualBase::draw(){
        if( useCapture && !captureInput.captureAvailable() ){
            return;
        }
        AppBase::draw();
    }
    
    void AppMultiVisualBase::changeVisual( int index ){
        
        
        if( index == mCurrentVisualIndex ){
            mNextVisualIndex = -1;
            return;
        }
        
        if( index < 0 ){
            index = 0;
        }else if( mVisuals.size() <= index ){
            index = mVisuals.size() - 1;
        }
        
        // トランジション処理中の場合完了を待つ.
        if( !mTransition->isCompleted() ){
            mNextVisualIndex = index;
            return;
        }else{
            mNextVisualIndex = -1;
        }
        
        mTransition->setup( mVisuals.at(mCurrentVisualIndex), mVisuals.at(index) );
        
        mCurrentVisualIndex = index;
        
    }
    
    void AppMultiVisualBase::keyDown( ci::app::KeyEvent event ){
        if( event.getChar() == 'f' ){
            ci::app::setFullScreen( ! ci::app::isFullScreen() );
        }
        else if( event.getChar() == 'r' ){
            changeVisual( randInt( mVisuals.size() ) );
        }
        else if( event.getChar() == 's' ){
            int idx = mCurrentVisualIndex + 1;
            if( mVisuals.size() <= idx ){
                idx = 0;
            }
            changeVisual(idx);
        }
    }
    
    void AppMultiVisualBase::setupOsc(){
        AppBase::setupOsc();
        // ===========================================
        // == Write osc code if needed.
        
        // ===========================================
    }
    
}}