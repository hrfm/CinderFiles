#pragma once

// === INCLUDE ====================================================================================================

#include <list>
#include "Resources.h"
#include "AppBase.h"
#include "VisualBase.h"
#include "VisualTransitionBase.h"

// === NAMESPACE ===================================================================================================

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace hrfm::visual;

// === AppMultiVisualBase ==========================================================================================

// 複数の VisualBase を扱うためのクラス.
// Transition を設定する事で間をつなぐ事も出来る.
namespace hrfm{ namespace app{
    
    class AppMultiVisualBase : public hrfm::app::AppBase {
    
    public:
        
        void prepareSettings( Settings *settings );
        
        void setup();
        
        void setupTransition( VisualTransitionBase * transition );
        
        void update();
        
        void draw();
        
        void changeVisual( int index );
        
        virtual void keyDown( ci::app::KeyEvent event );
    
    protected :
        
        vector<VisualBase*>     mVisuals;
    
        int                     mCurrentVisualIndex;
        int                     mNextVisualIndex;
    
        bool                    mUseTransition;
        VisualTransitionBase *  mTransition;
    
        gl::Texture             mTexture;
        
        void setupOsc();
        
    };
    
}}