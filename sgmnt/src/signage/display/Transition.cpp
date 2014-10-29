#include "Transition.h"

using namespace sgmnt::display;
using namespace sgmnt::events;

namespace sgmnt{ namespace signage{ namespace display{
    
    Transition::Transition(){
        EventDispatcher();
        FboTexture();
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
        FboTexture::init(size);
    }
    
    void Transition::setup( sgmnt::display::Texture * tex0, sgmnt::display::Texture * tex1, float time ){
        _tex0 = tex0;
        _tex1 = tex1;
        _time = time;
        _startedAt = ci::app::getElapsedSeconds();
    }
    
    void Transition::_update(){
        float elapsed = ci::app::getElapsedSeconds() - _startedAt;
        if( _time < elapsed ){
            dispatchEvent( new sgmnt::events::Event( sgmnt::events::Event::COMPLETE ) );
        }
    }
    
    void Transition::_draw(){}
    
    void Transition::setSize( float width, float height ){
        FboTexture::setSize( width, height );
    }
    
    bool Transition::running(){
        return true;
    }
    
}}}