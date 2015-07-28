#pragma once

#include "hrfm.h"
#include "FilterBase.h"
#include "SiBPM.h"

namespace hrfm{ namespace vj{

    class BeatFilterBase : public hrfm::gl::FilterBase{
        
    public:
        
        BeatFilterBase() : FilterBase(){}
        BeatFilterBase( string fragmentShader ) : FilterBase( fragmentShader ){}
        BeatFilterBase( string fragmentShader, string vertexShader ) : FilterBase( fragmentShader, vertexShader ){}
        
        virtual void update(){}
        
    protected:
        
        virtual void prepare(){
            int position = SiBPM::getInstance().position * 1000000;
            mShader.uniform( "bpm_position", position / 1000000.0f );
        }
        
    };
    
}}