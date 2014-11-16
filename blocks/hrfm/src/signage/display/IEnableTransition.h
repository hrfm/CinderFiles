#pragma once

#include "cinder/app/App.h"
#include "hrfm.h"
#include "Transition.h"

namespace hrfm{ namespace signage{ namespace display{
    
    class IEnableTransition{
        
    public:
        
        IEnableTransition(){}
        ~IEnableTransition(){};
        
        virtual void setTransition( hrfm::signage::display::Transition * transition ){
            _transition = transition;
        }
        
        virtual void setupTransition(  hrfm::display::TextureNode * tex0, hrfm::display::TextureNode * tex1, float time ){
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