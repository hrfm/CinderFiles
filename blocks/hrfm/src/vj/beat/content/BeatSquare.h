#pragma once

#include "BeatContentBase.h"

namespace hrfm{ namespace vj{
    
    class BeatSquare : public hrfm::vj::BeatContentBase{
    public:
        BeatSquare(){
            hrfm::vj::BeatContentBase();
        }
        virtual void draw( double position, int width, int height );
    };
    
    class BeatLine : public hrfm::vj::BeatContentBase{
    public:
        BeatLine(){
            hrfm::vj::BeatContentBase();
        }
        virtual void draw( double position, int width, int height );
    };
    
    class BeatCircle : public hrfm::vj::BeatContentBase{
    public:
        BeatCircle(){
            hrfm::vj::BeatContentBase();
        }
        virtual void draw( double position, int width, int height );
    };
    
}}