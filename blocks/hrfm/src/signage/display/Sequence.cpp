#include "Sequence.h"

using namespace hrfm::display;
using namespace hrfm::events;
using namespace hrfm::signage::display;

namespace hrfm{ namespace signage{ namespace display{
    
    //! public:
    
    Sequence::Sequence(){
        EventDispatcher();
        _trigger = "";
    }
    
    Sequence::Sequence( hrfm::display::IDrawable * content, float time ){
        Sequence();
        _content = content;
        _time    = time;
        if( _time <= 0.0f ){
            if( EventDispatcher * dispatcher = dynamic_cast<EventDispatcher*>(content) ){
                dispatcher->addEventListener( hrfm::events::Event::COMPLETE, this, &Sequence::_onContentComplete );
            }else{
                throw "Invalid Sequence Content.";
            }
        }
    }
    
    Sequence::~Sequence(){
        
    }
    
    void Sequence::setTrigger( string trigger ){
        _trigger = trigger;
    }
    
    hrfm::display::IDrawable * Sequence::getContentRef(){
        return _content;
    }
    
    void Sequence::play(){
        
        _startedAt = ci::app::getElapsedSeconds();
        
        if( _trigger != "" ){
            hrfm::events::TriggerEvent * evt = new hrfm::events::TriggerEvent( hrfm::events::TriggerEvent::TRIGGER, _trigger );
            dispatchEvent( evt );
            hrfm::events::SiEventDispatcher::getInstance().dispatchEvent( evt );
        }
        
        if( MovieTexture * mov = dynamic_cast<MovieTexture*>(_content) ){
            mov->play();
        }else if( SequentialContents * seq = dynamic_cast<SequentialContents*>(_content) ){
            seq->play();
        }
        
    }
    
    void Sequence::stop(){
        _startedAt = ci::app::getElapsedSeconds();
        if( MovieTexture * mov = dynamic_cast<MovieTexture*>(_content) ){
            mov->stop();
        }else if( SequentialContents * seq = dynamic_cast<SequentialContents*>(_content) ){
            seq->stop();
        }
    }
    
    void Sequence::update(){
        if( 0.0f < _time ){
            float elapsed = ci::app::getElapsedSeconds() - _startedAt;
            if( _time < elapsed ){
                dispatchEvent( new hrfm::events::Event( hrfm::events::Event::COMPLETE ) );
            }
        }
    }
    
    void Sequence::setSize( float width, float height ){
        if(_content){
            _content->setSize( width, height );
        }
    }
    
    //! protected:
    
    void Sequence::_onContentComplete( hrfm::events::Event * event ){
        dispatchEvent( new hrfm::events::Event( hrfm::events::Event::COMPLETE ) );
    }
    
}}}