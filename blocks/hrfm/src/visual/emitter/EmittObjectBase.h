#pragma once

#include "EmitterBase.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace hrfm::events;

namespace hrfm{ namespace visual{ namespace emitter{
    
    class EmittObjectBase : public EmitterBase{
        
    public:
        
        EmittObjectBase(){
            EmitterBase();
        }
        
        virtual void init( float time ){
            EmitterBase::init();
            this->animateTime = time;
            this->startTime = getElapsedSeconds();
            addEventListener( hrfm::events::Event::ADDED_TO_STAGE, this, &EmittObjectBase::_onAddedToStage );
        }
        
        virtual void update(){
            float elapsedTime = getElapsedSeconds();
            progress = ( elapsedTime - startTime ) / animateTime;
            if( 1.0f < progress ){
                removeOwn();
            }else{
                hrfm::display::DisplayNode::update();
            }
        }
        
        float animateTime, startTime, progress;
        
    protected:
        
        virtual void _onAddedToStage( hrfm::events::Event * event ){
            this->startTime = getElapsedSeconds();
            removeEventListener( hrfm::events::Event::ADDED_TO_STAGE, this, &EmittObjectBase::_onAddedToStage );
        }
        
    };
    
}}};