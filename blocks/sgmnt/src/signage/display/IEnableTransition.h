#pragma once

#include "cinder/app/App.h"
#include "sgmnt.h"
#include "Transition.h"

namespace sgmnt{ namespace signage{ namespace display{
    
    class IEnableTransition : public sgmnt::events::EventDispatcher{
        
    public:
        
        IEnableTransition(){
            sgmnt::events::EventDispatcher();
        }
        ~IEnableTransition(){};
        
        virtual void setTransition( sgmnt::signage::display::Transition * transition ){
            _transition = transition;
        }
        
        virtual void setupTransition(  sgmnt::display::Texture * tex0, sgmnt::display::Texture * tex1, float time ){
            if( _transition ){
                _transition->setup( tex0, tex1, time );
            }
        }
        
        virtual Transition * getTransitionRef(){
            return _transition;
        }
        
        virtual bool isTransitionRunning(){
            if( _transition ){
                return _transition->running();
            }else{
                return false;
            }
        }
        
        virtual void updateTransition(){
            if(isTransitionRunning()){
                _transition->update();
            }
        }
        
        virtual void drawTransition(){
            if(isTransitionRunning()){
                _transition->draw();
            }
        }
        
    protected:
        
        Transition * _transition;
        
    };
    
}}}