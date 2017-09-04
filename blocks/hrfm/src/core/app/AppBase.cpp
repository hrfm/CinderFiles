#include "AppBase.h"
#include <iostream>
#include "cinder/Utilities.h"

// === NAMESPACE ===================================================================================================

using namespace std;
using namespace hrfm;
using namespace hrfm::app;
using namespace hrfm::events;
using namespace hrfm::io;

// === GLOBAL PROPERTY =============================================================================================

namespace hrfm { namespace app{
    
    void AppBase::setup(){
        hrfm::app::AppInstallation::setup();
        mWindowSize = SiAppSettings::getInstance().getWindowSize();
        stage = hrfm::display::Stage::create();
        resize();
    }
    
    // === Setter / Getter =======================================================
    
    ivec2 AppBase::getWindowSize(){
        return mWindowSize;
    }
    
    // === Update / Draw =========================================================
    
    void AppBase::update(){
        hrfm::app::AppInstallation::update();
        updateStage();
    }
    
    void AppBase::updateStage(){
        vector<DisplayNode*>::iterator it;
        vector<DisplayNode*>::iterator end = stage->children.end();
        for( it=stage->children.begin(); it!=end; ++it ){
            (*it)->size( getWindowSize() );
        }
        stage->update();
        stage->draw();
    }
    
    void AppBase::draw(){
        ci::gl::clear();
        ci::gl::draw( stage->getTexture(), getWindowBounds() );
        if( _isDebugMode ){
            //cout << "debug" << endl;
            _drawDebug();
        }
    }
    
    // === Event Listener. =======================================================
    
    void AppBase::resize(){
        camera.setPerspective( 30, getWindowAspectRatio(), 0.1, 100 );
        stage->size( getWindowSize() );
    }
    
    //!private
    
}}
