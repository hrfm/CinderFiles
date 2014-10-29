#include "Sequence.h"

using namespace sgmnt::display;
using namespace sgmnt::events;

namespace sgmnt{ namespace signage{ namespace display{
    
    //! public:
    
    Sequence::Sequence(){
        EventDispatcher();
    }
    
    Sequence::Sequence( sgmnt::display::IDrawable * content, float time ){
        Sequence();
        cout << content << endl;
        _content = content;
        _time    = time;
        if( _time <= 0.0f ){
            if( EventDispatcher * dispatcher = dynamic_cast<EventDispatcher*>(content) ){
                dispatcher->addEventListener( Event::COMPLETE, this, &Sequence::_onContentComplete );
            }else{
                throw "Invalid Sequence Content.";
            }
        }
    }
    
    Sequence::~Sequence(){
        
    }
    
    sgmnt::display::IDrawable * Sequence::getContentRef(){
        return _content;
    }
    
    void Sequence::play(){
        _startedAt = ci::app::getElapsedSeconds();
        _play( _content );
    }
    
    void Sequence::update(){
        if( 0.0f < _time ){
            _update( _content );
        }
    }
    
    //! protected:
    
    void Sequence::_play( MovieTexture * content ){
        ci::qtime::MovieGlRef ref = content->getMovieGlRef();
        ref->seekToStart();
        ref->play();
    }
    
    void Sequence::_play( IDrawable * content ){
        if( MovieTexture * mov = dynamic_cast<MovieTexture*>(content) ){
            _play( mov );
        }
    }
    
    void Sequence::_update( IDrawable * content ){
        float elapsed = ci::app::getElapsedSeconds() - _startedAt;
        if( _time < elapsed ){
            dispatchEvent( new sgmnt::events::Event( sgmnt::events::Event::COMPLETE ) );
        }
    }
    
    void Sequence::_onContentComplete( sgmnt::events::Event * event ){
        dispatchEvent( new sgmnt::events::Event( sgmnt::events::Event::COMPLETE ) );
    }
    
}}}