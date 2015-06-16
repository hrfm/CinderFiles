#pragma once

#include "VisualBase.h"

using namespace ci;
using namespace std;

using namespace hrfm;
using namespace hrfm::visual;

namespace hrfm{ namespace visual{
    
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