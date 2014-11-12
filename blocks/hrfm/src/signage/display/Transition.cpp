#include "Transition.h"

using namespace hrfm::display;
using namespace hrfm::events;

namespace hrfm{ namespace signage{ namespace display{
    
    Transition::Transition(){
        hrfm::display::Texture();
    };
    Transition::Transition( int width, int height ){
        Transition( Vec2i( width, height ) );
    }
    Transition::Transition( ci::Vec2i size ){
        Transition();
        init( size );
    }
    Transition::~Transition(){}
    
    void Transition::init( ci::Vec2i size ){
        //FboTexture::init(size);
    }
    
    void Transition::setup( hrfm::display::Texture * tex0, hrfm::display::Texture * tex1, float time ){
        _tex0 = tex0;
        _tex1 = tex1;
        _time = time;
        _startedAt = ci::app::getElapsedSeconds();
    }
    
    void Transition::_update(){
        float elapsed = ci::app::getElapsedSeconds() - _startedAt;
        if( _time < elapsed ){
            dispatchEvent( new hrfm::events::Event( hrfm::events::Event::COMPLETE ) );
        }
    }
    
    void Transition::_draw(){}
    
    void Transition::setSize( float width, float height ){
        //FboTexture::setSize( width, height );
    }
    
    bool Transition::running(){
        return true;
    }
    
}}}