#pragma once

#include "EmitterBase.h"

using namespace ci;
using namespace std;
using namespace hrfm::events;

namespace hrfm{ namespace visual{ namespace emitter{
    
    class EmittObjectBase : public EmitterBase{
        
    public:
        
        EmittObjectBase(){
            
            EmitterBase();
            
            _alphaFrom = 1.0f;
            _alphaTo   = 0.0f;
            _alpha     = 1.0f;
            
        }
        
        virtual void init( float time ){
            EmitterBase::init();
            this->animateTime = time;
            this->startTime = ci::app::getElapsedSeconds();
            addEventListener( hrfm::events::Event::ADDED_TO_STAGE, this, &EmittObjectBase::_onAddedToStage );
        }
        
        virtual void update(){
            float elapsedTime = ci::app::getElapsedSeconds();
            progress = ( elapsedTime - startTime ) / animateTime;
            if( 1.0f < progress ){
                removeOwn();
            }else{
                hrfm::display::DisplayNode::update();
            }
        }
        
        float animateTime, startTime, progress;
        
    protected:
        
        virtual void _update(){
            hrfm::display::DisplayNode::_update();
            _alpha = _alphaFrom + ( _alphaTo - _alphaFrom ) * progress;
        }
        
        virtual void _onAddedToStage( hrfm::events::Event * event ){
            this->startTime = ci::app::getElapsedSeconds();
            removeEventListener( hrfm::events::Event::ADDED_TO_STAGE, this, &EmittObjectBase::_onAddedToStage );
        }
        
        const float _PI = 3.14159265358979323846;
        
        float _alphaFrom;
        float _alphaTo;
        float _alpha;
        
    };
    
}}};