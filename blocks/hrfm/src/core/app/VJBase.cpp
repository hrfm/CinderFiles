#include "VJBase.h"

namespace hrfm { namespace app{
    
    void VJBase::setup(){
        
        hrfm::app::AppBase::setup();
        
        hrfm::io::SiKORGMIDIInterface::getInstance().addEventListener(
            hrfm::events::KORGMIDIInterfaceEvent::PAD_ON, this, &VJBase::onPadOn
        );
        hrfm::io::SiKORGMIDIInterface::getInstance().addEventListener(
            hrfm::events::KORGMIDIInterfaceEvent::PAD_OFF, this, &VJBase::onPadOff
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
            _guide->visible = !_guide->visible;
        }
        
        if( event.getCode() == event.KEY_SPACE ){
            _updateBPM();
        }
        
    }
    
    void VJBase::update(){
        hrfm::app::AppBase::update();
        hrfm::vj::SiBPM::getInstance().update();
    }
    
    void VJBase::draw(){
        hrfm::app::AppBase::draw();
    }
    
    void VJBase::onPadOn( hrfm::events::KORGMIDIInterfaceEvent * event ){
        _updateBPM();
    }
    
    void VJBase::onPadOff( hrfm::events::KORGMIDIInterfaceEvent * event ){}
    
    // private
    
    void VJBase::_updateBPM(){
        double now = ci::app::AppNative::getElapsedSeconds();
        double elapsedTime = now - _recentPressSpaceTime;
        if( elapsedTime < 3.0 ){
            hrfm::vj::SiBPM::getInstance().setBPM(60.0/elapsedTime);
            //cout << elapsedTime << endl;
        }
        _recentPressSpaceTime = now;
    }
    
}}