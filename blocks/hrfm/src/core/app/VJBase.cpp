#include "VJBase.h"

namespace hrfm { namespace app{
    
    void VJBase::setup(){
        hrfm::app::AppBase::setup();
        hrfm::io::SiKORGMIDIInterface::getInstance().addEventListener(
            hrfm::events::KORGMIDIInterfaceEvent::PAD_ON, this, &VJBase::_onPadOn
        );
    }
    
    void VJBase::setGuide( fs::path filePath ){
        _guide = new hrfm::display::ImageTexture( loadImage(hrfm::utils::DataLoader::load(filePath)) );
        _guide->visible = false;
        stage.addChild( _guide );
    }
    
    void VJBase::keyDown( ci::app::KeyEvent event ){
        hrfm::app::AppBase::keyDown(event);
        if( _guide && event.getCode() == event.KEY_g ){
            cout << "hoge" << endl;
            _guide->visible = !_guide->visible;
        }
    }
    
    void VJBase::update(){
        vector<DisplayNode*>::iterator it;
        vector<DisplayNode*>::iterator end = stage.children.end();
        for( it=stage.children.begin(); it!=end; ++it ){
            (*it)->setSize( getWindowWidth(), getWindowHeight() );
        }
        hrfm::app::AppBase::update();
    }
    
    void VJBase::draw(){
        hrfm::app::AppBase::draw();
    }
    
    // private
    
    void VJBase::_onPadOn( hrfm::events::KORGMIDIInterfaceEvent * event ){
        
        /*
        float t = getElapsedSeconds();
        
        if( event->id == 15 ){
            if( 30.0 > t - beforeTime ){
                interval += t - beforeTime;
                interval /= 2.0;
                cout << interval << endl;
            }
            beforeTime = t;
        }
        
        if( event->id < _contentList.size() ){
            //change( event->id );
        }
        //*/
        
    }
    
}}