#pragma once

#include "hrfm.h"

namespace hrfm{ namespace vj{
    
    class BeatContentBase : public hrfm::display::DisplayNode{
        
    public:
        
        BeatContentBase(){
            hrfm::display::DisplayNode();
        }
        
        virtual void draw( double position, int width, int height ){};
        
    private:
        
    };
    
}}