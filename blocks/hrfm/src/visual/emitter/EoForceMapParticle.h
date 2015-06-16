#pragma once

#include "hrfm.h"
#include "EmittObjectBase.h"

using namespace ci;
using namespace std;
using namespace hrfm::events;

namespace hrfm{ namespace visual{ namespace emitter{
    
    class EoForceMapParticle :public EmittObjectBase{
        
    public:
        
        EoForceMapParticle();
        EoForceMapParticle( float time, hrfm::cv::ForceMap * map );
        
        virtual void init( float time, hrfm::cv::ForceMap * map );
        
        float getForceLength();
        
    protected:
        
        virtual void _update();
        virtual void _draw();
        
        float _forceX;
        float _forceY;
        float _forceLength;
        hrfm::cv::ForceMap * _forceMap = NULL;
        
    private:
        
    };
    
}}};