#pragma once

#include "VboWall.h"

namespace hrfm{ namespace gl{
    
    class VboSphere : public VboWall{
    
    public:
        
        VboSphere():VboWall(){};
        ~VboSphere(){};
        
        virtual void setup( Vec2i segments, bool curve = false, float noise = false );
        
    protected:
        
    };
    
}}