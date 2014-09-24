#pragma once

#include "VisualBase.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace sgmnt;
using namespace sgmnt::visual;

namespace sgmnt{ namespace visual{
    
    class ZeroTransition : public VisualTransitionBase{
        
        public :
        
            ZeroTransition(){
                VisualTransitionBase();
            };
        
            void init( Vec2i size, float transitionTime ){
                VisualTransitionBase::init( size, 0.0 );
            };
            
        private :
            
    };
    
}}