#pragma once

#include "BeatFilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm{ namespace vj{

    class BeatFilterScroll : public hrfm::vj::BeatFilterBase{
    
    public:
        
        BeatFilterScroll( float scrollX = 0.0, float scrollY = 0.0 ) : BeatFilterBase("ScrollFilter.glsl"){
            _scrollX = scrollX;
            _scrollY = scrollY;
        };
        
        virtual void _update( BeatContentBase * content ){
            BeatFilterBase::_update( content );
        }
        
    protected:
        
        virtual void prepare(){
            BeatFilterBase::prepare();
            mShader.uniform( "scrollX", _scrollX );
            mShader.uniform( "scrollY", _scrollY );
        }
        
    private:
        
        float _scrollX;
        float _scrollY;
        
    };

}}