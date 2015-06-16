#include "Sequence.h"

namespace hrfm{ namespace signage{ namespace display{
    
    //! public:
    
    Sequence::Sequence(){
        EventDispatcher();
        _trigger = "";
    }
    
    Sequence::Sequence( hrfm::display::DisplayNode * content, float duration ){
        
        Sequence();
        
        _content  = content;
        _duration = duration;
        
        if( _duration <= 0.0f ){
            if( EventDispatcher * dispatcher = dynamic_cast<EventDispatcher*>(content) ){
                dispatcher->addEventListener( hrfm::events::Event::COMPLETE, this, &Sequence::_onContentComplete );
            }else{
                throw "Invalid Sequence Content.";
            }
        }
        
        if( hrfm::utils::instanceof<::hrfm::display::MovieTexture>(*content) ){
            _type = hrfm::utils::FILE_TYPE_MOV;
        }else if( hrfm::utils::instanceof<::hrfm::display::ImageTexture>(*content) ){
            _type = hrfm::utils::FILE_TYPE_PIC;
        }else if( hrfm::utils::instanceof<SequentialContents>(*content) ){
            _type = "seq";
        }else if( hrfm::utils::instanceof<::hrfm::display::DisplayNode>(*content) ){
            _type = "blank";
        }else{
            _type = "unknown";
        }
        
    }
    
    Sequence::~Sequence(){}
    
    string Sequence::getType(){
        return _type;
    }
    
    void Sequence::setTrigger( string trigger ){
        _trigger = trigger;
    }
    
    hrfm::display::DisplayNode * Sequence::getContentRef(){
        return _content;
    }
    
    void Sequence::play(){
        
        _startedAt = ci::app::getElapsedSeconds();
        
        if( _trigger != "" )
        {
            hrfm::events::TriggerEvent * evt = new hrfm::events::TriggerEvent( hrfm::events::TriggerEvent::TRIGGER, _trigger );
            dispatchEvent( evt );
            hrfm::events::SiEventDispatcher::getInstance().dispatchEvent( evt );
        }
        
        if( ::hrfm::display::MovieTexture * mov = dynamic_cast<::hrfm::display::MovieTexture*>(_content) )
        {
            mov->play();
        }
        else if( hrfm::signage::display::SequentialContents * seq = dynamic_cast<SequentialContents*>(_content) )
        {
            seq->play();
        }
        
    }
    
    void Sequence::stop(){
        _startedAt = ci::app::getElapsedSeconds();
        if( ::hrfm::display::MovieTexture * mov = dynamic_cast<::hrfm::display::MovieTexture*>(_content) )
        {
            mov->stop();
        }
        else if( hrfm::signage::display::SequentialContents * seq = dynamic_cast<SequentialContents*>(_content) )
        {
            seq->stop();
        }
        
    }
    
    void Sequence::update(){
        if( 0.0f < _duration )
        {
            float elapsed = ci::app::getElapsedSeconds() - _startedAt;
            if( _duration < elapsed )
            {
                dispatchEvent( new hrfm::events::Event( hrfm::events::Event::COMPLETE ) );
            }
        }
    }
    
    void Sequence::setSize( float width, float height ){
        if(_content)
        {
            _content->setSize( width, height );
        }
    }
    
    //! protected:
    
    void Sequence::_onContentComplete( hrfm::events::Event * event ){
        dispatchEvent( new hrfm::events::Event( hrfm::events::Event::COMPLETE ) );
    }
    
}}}