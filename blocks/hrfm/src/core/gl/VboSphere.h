#pragma once

#include "VboWall.h"

namespace hrfm{ namespace gl{
    
    class VboSphere : public VboWall{
    
    public:
        
        VboSphere():VboWall(){};
        ~VboSphere(){};
        
        virtual void setup( ivec2 segments, bool curve = false, float noise = false );
        
    protected:
        
    };
    
}}