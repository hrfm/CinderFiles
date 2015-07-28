#pragma once

#include "hrfm.h"
#include "FilterBase.h"

namespace hrfm{ namespace vj{

    class BeatFilterBase : public hrfm::gl::FilterBase{
        
    public:
        
        BeatFilterBase(){
            hrfm::gl::FilterBase();
        }
        
        virtual void update(){
            
        }
        
    protected:
        
        /*
        virtual void prepare(){
            mShader.uniform( "beat",  );
            mShader.uniform( "r_scale", r_scale );
            mShader.uniform( "r_min", r_min );
            mShader.uniform( "r_max", r_max );
        }
        //*/
        
    };
    
}}