#pragma once

#include "EventDispatcher.h"

using namespace std;

namespace sgmnt{ namespace utils{
    
    class Timer : public sgmnt::events::EventDispatcher{
        
    public:
        
        Timer();
        ~Timer();
        
    private:
        
    };
    
}}