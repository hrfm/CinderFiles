#include "VJBase.h"

namespace hrfm { namespace app{
    
    void VJBase::setup(){
        
        hrfm::app::AppBase::setup();
        
        hrfm::io::SiKORGMIDIInterface::getInstance().addEventListener(
            hrfm::events::KORGMIDIInterfaceEvent::PAD_ON, this, &VJBase::_onPadOn
        );
        
        hrfm::vj::SiBPM::getInstance().start();
        
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
        
        if( event.getCode() == event.KEY_SPACE ){
            double now = ci::app::AppNative::getElapsedSeconds();
            double elapsedTime = now - _recentPressSpaceTime;
            if( elapsedTime < 3.0 ){
                hrfm::vj::SiBPM::getInstance().setBPM(60.0/elapsedTime);
                //cout << elapsedTime << endl;
            } 
            _recentPressSpaceTime = now;
        }
        
    }
    
    void VJBase::update(){
        vector<DisplayNode*>::iterator it;
        vector<DisplayNode*>::iterator end = stage.children.end();
        for( it=stage.children.begin(); it!=end; ++it ){
            (*it)->setSize( getWindowWidth(), getWindowHeight() );
        }
        hrfm::vj::SiBPM::getInstance().update();
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